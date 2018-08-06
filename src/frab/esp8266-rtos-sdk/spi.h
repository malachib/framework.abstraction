#pragma once

// large portions lifted and adapted from Arduino
// good reference here https://github.com/MetalPhreak/ESP8266_SPI_Driver/blob/master/driver/spi.c
//   also
// http://d.av.id.au/blog/hardware-spi-hspi-command-data-registers/

// FIX: nasty un-doing of framework_abstraction namespace
}

extern "C" {

#include "peri.h"
#include "driver/gpio.h"
#include "driver/spi_interface.h"
#include "driver/spi_register.h"
#include "esp_log.h"
#include <string.h> // for memcpy

}

// FIX: nasty re-doing of namespace (because we are included externally)
namespace framework_abstraction {

namespace layer1 {

template <SpiNum spi>
struct spi_traits;

template <>
struct spi_traits<SpiNum_HSPI>
{
    typedef size_t size_type;
    static volatile uint32_t& W0() { return SPI1W0; }
    static void U1(uint32_t value) { SPI1U1 = value; }
    static uint32_t U1() { return SPI1U1; }
    static volatile uint32_t& CMD() { return SPI1CMD; }
    static volatile uint32_t& C() { return SPI1C; }
    static volatile uint32_t& U() { return SPI1U; }
    static void U(uint32_t value) { SPI1U = value; }
    static void C(uint32_t value) { SPI1C = value; }
    static void C1(uint32_t value) { SPI1C1 = value; }

    static void setup_pins()
    {
        PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_HSPIQ_MISO);
        PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_HSPI_CS0);
        PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_HSPID_MOSI);
        PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_HSPI_CLK);
    }

    // experimental, this indicates a special data() method is available
    // to access a FIFO (or perhaps DMA, on another chip) buffer
    typedef void has_output_buffer_tag;
};

static const char* SPI_TAG = "layer1::SPI";

template <SpiNum spi, class TSpiTraits = spi_traits<spi>, class TPolicy = void >
class SPI
{
    typedef TSpiTraits spi_traits;
    typedef typename spi_traits::size_type size_type;
    typedef TPolicy policy_type;

    static constexpr int fifo_size = 64;

    inline static void set_data_bits(uint16_t bits)
    {
        const uint32_t mask = ~((SPIMMOSI << SPILMOSI) | (SPIMMISO << SPILMISO));
        bits--;
        spi_traits::U1(((spi_traits::U1() & mask) | ((bits << SPILMOSI) | (bits << SPILMISO))));
    }

public:
    static void commit()
    {
        spi_traits::CMD() |= SPIBUSY;
        // 90% sure these lines do the same thing
        SET_PERI_REG_MASK(SPI_CMD(spi), SPI_USR);
    }
    
    static void busy_wait(bool auto_commit = true)
    {
        while(spi_traits::CMD() & SPIBUSY) {}        
    }

private:
    static void write_fifo_dummy_aligned(size_type size)
    {
        set_data_bits(size * 8);
        uint32_t dataSize = ((size + 3) / 4);
        volatile uint32_t* fifoPtr = &spi_traits::W0();

        while(dataSize--)
        {
            *fifoPtr = 0xFFFFFFFF;
            fifoPtr++;
        }
    }

    // demands byte aligned copy as well
    // TODO: Make this a non-inline function to avoid code bloat
    static void write_fifo_aligned(const void* source, size_type size)
    {
        busy_wait();
        set_data_bits(size * 8);

        volatile uint32_t* fifoPtr = &spi_traits::W0();

        const uint32_t * dataPtr = (uint32_t*) source;
        uint32_t dataSize = ((size + 3) / 4);

        while(dataSize--)
            *fifoPtr++ = *dataPtr++;

        __sync_synchronize();
        commit();
    }


    // does not need to be 32-bit aligned
    // also, as this is SPI, reads must follow writes
    static void read_fifo(uint8_t* dest, uint8_t size)
    {
        volatile uint8_t * fifoPtr8 = (volatile uint8_t *) &spi_traits::W0();
        while(size--) 
            *dest++ = *fifoPtr8++;
    }

    // NOTE: Must be 32-bit aligned
    static void write_aligned(const void* source, size_type size)
    {
        uint8_t* s = (uint8_t*)source;

        while(size)
        {
            if(size > fifo_size)
            {
                write_fifo_aligned(s, fifo_size);
                size -= fifo_size;
                s += fifo_size;
            }
            else 
            {
                write_fifo_aligned(s, size);
                size = 0;
            }
        }
    }
public:
    static inline void write8(uint8_t value, bool wait = true)
    {
        if(wait)    busy_wait();
        set_data_bits(8);
        spi_traits::W0() = value;
        commit();
    }

    // NOTE: this only interacts with the 'dout' area. remember esp8266 likes to
    // transfer cmd+addr+dout.  So either set up your cmd+addr beforehand or
    // disable them
    uint8_t* data()
    {
        volatile uint8_t * fifoPtr8 = (volatile uint8_t *) &spi_traits::W0();

        return fifoPtr8;
    }


private:
    // only use this for small writes, up to 4 bytes max
    static void write_unaligned(const uint8_t* source, uint8_t size)
    {
        set_data_bits(size * 8);

        union 
        {
            uint32_t ensure_32_bit;
            uint8_t s[4];
        };

        // FIX: Endianness is gonna follow CPU arch here, which I am pretty
        // sure is big endian for xtensa
        memcpy(s, source, size);

        spi_traits::W0() = ensure_32_bit;
    }

    static int16_t read8()
    {
        return spi_traits::W0();
    }

    // must be 32-bit aligned [mainly output]
    static void transfer_fifo(const uint8_t* out, uint8_t* in, uint8_t size)
    {
        if(out != nullptr)
            write_fifo_aligned(out, size);
        else
            write_fifo_dummy_aligned(size);

        commit();
        busy_wait();

        if(in != nullptr)
            read_fifo(out, size);
    }

public:
    inline static void begin()
    {
        // TODO: set up pin modes also

        spi_traits::setup_pins();

        spi_traits::C(0);
        spi_traits::U(SPIUMOSI | SPIUDUPLEX | SPIUSSE);
        spi_traits::U1((7 << SPILMOSI) | (7 << SPILMISO));
        spi_traits::C1(0);
    }


    // TODO: Utilize spi_endianness_t
    inline static void set_order_msb(bool msb_first)
    {
        if(msb_first)
            spi_traits::C() &= ~(SPICWBO | SPICRBO);
        else
            spi_traits::C() |= ~(SPICWBO | SPICRBO);
    }

    static void write(const void* source, size_type size)
    {
        uint8_t* s = (uint8_t*)source;
        uint32_t raw_addr = (uint32_t)s;

//#ifdef DEBUG
        int extra = raw_addr % 4;
        size -= extra;
        if(extra != 0)
        {
            // TODO: actually align these bytes
            ESP_LOGE(SPI_TAG, "Aligning bytes for write call ONLY in debug mode");
            write_unaligned(s, extra);
            s += extra;
        }
//#endif        

        write_aligned(s, size);
    }

    // must be 32-bit aligned
    // TODO: Make this non-inline
    static void transfer_aligned(const void* source, void* dest, size_type size)
    {
        const uint8_t* s = (const uint8_t*) source;
        uint8_t* d = (uint8_t*) dest;

        while(size > 0)
        {
            if(size > fifo_size)
            {
                transfer_fifo(s, d, fifo_size);

                if(s != nullptr) s += fifo_size;
                if(d != nullptr) d += fifo_size;

                size -= fifo_size;
            }
            else
            {
                transfer_fifo(s, d, size);
                size = 0;
            }
        }
    }


    static void transfer(const void* source, void* dest, size_type size)
    {
        const uint8_t* s = (const uint8_t*)source;
        uint8_t* d = (uint8_t*)dest;
        uint32_t raw_addr = (uint32_t)s;

//#ifdef DEBUG
        int extra = raw_addr % 4;
        if(extra != 0)
        {
            // TODO: actually align these bytes
            ESP_LOGE(SPI_TAG, "Aligning bytes for transfer call ONLY in debug mode");
            while(extra--)
            {
                write8(*s++);
                *d++ = read8();
            }
        }
//#endif

        transfer_aligned(s, d, size);
    }
};

}
// NOTE: Seems very, very likely they will be changing this to a more esp-idf API at some point
// so be ready to check IDF_VER macro and dispatch accordingly

// ultra low level, but good data here:
// https://github.com/esp8266/Arduino/blob/master/libraries/SPI/SPI.cpp
// https://github.com/esp8266/Arduino/blob/master/cores/esp8266/esp8266_peri.h

template <>
class spi_context<SpiNum>
{

};


/*
 * TODO: Still need to iron out where initializers go
template <>
inline SPI<SpiNum>::SPI()
{
    // Pretty sure this block is hard-wired to 'SpiNum_HSPI' interface.  May want to interrogate SpiNum
    // to see what we really wanna do here
    //Initialze Pins on ESP8266
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_HSPIQ_MISO);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_HSPI_CS0);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_HSPID_MOSI);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_HSPI_CLK);

    SpiAttr pAttr;   //Set as Master/Sub mode 0 and speed 10MHz
    pAttr.mode = SpiMode_Master;
    pAttr.subMode = SpiSubMode_0;
    pAttr.speed = SpiSpeed_10MHz;
    pAttr.bitOrder = SpiBitOrder_MSBFirst;
    // TODO: Change SpiNum_HSPI to this->spi at some point, though
    // for ESP8266 prtty sure we are mostly interested in the non-flash SPI
    // (which I *think* actually is the NON HSPI port)
    SPIInit(SpiNum_HSPI, &pAttr);    
}
*/

template <>
inline void SPI<SpiNum>::begin()
{

}


template <>
inline int SPI<SpiNum>::transfer(uint16_t data)
{
    SpiData pDat;
    pDat.cmd = data;
    pDat.cmdLen = 2;		      ///< Command byte length
    pDat.addr = NULL; 		      ///< Point to address value
    pDat.addrLen = 0; 	          ///< Address byte length
    pDat.data = NULL; 		      ///< Point to data buffer
    pDat.dataLen = 0;  ///< Data byte length.
    SPIMasterSendData(SpiNum_HSPI, &pDat);

    // FIX: do proper retrieval of data here
    return -1;
}


template <>
inline void SPI<SpiNum>::transfer(const void* tx_buffer, size_t tx_length, void* rx_buffer, size_t rx_length)
{
    // FIX: Write only right now, and switching read/write here is a runtime check so that
    // wants to be optimized

    // dummy boilerplate.  Pausing because I never ran into formalized command/addr in SPI before
    SpiData pDat;
    pDat.cmdLen = 0;		      ///< Command byte length
    pDat.addr = NULL; 		      ///< Point to address value
    pDat.addrLen = 0; 	          ///< Address byte length
    // FIX: Need to ensure tx_buffer is actually uint32 aligned, this might be tricky
    // Also unclear what endian behavior is like in this situation
    pDat.data = (uint32_t*)tx_buffer; 		      ///< Point to data buffer
    pDat.dataLen = tx_length;  ///< Data byte length.
    SPIMasterSendData(SpiNum_HSPI, &pDat);
}


namespace hal
{

typedef framework_abstraction::SPI<SpiNum> SPI;

}