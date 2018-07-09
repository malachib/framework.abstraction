#pragma once

// large portions lifted and adapted from Arduino

extern "C" {

#include "peri.h"
#include "driver/spi_interface.h"
#include "driver/spi_register.h"
#include "esp_log.h"

}

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
};

static const char* SPI_TAG = "layer1::SPI";

template <SpiNum spi, class TSpiTraits = spi_traits<spi> >
class SPI
{
    typedef TSpiTraits spi_traits;
    typedef typename spi_traits::size_type size_type;
    static constexpr int fifo_size = 64;

    inline static void set_data_bits(uint16_t bits)
    {
        const uint32_t mask = ~((SPIMMOSI << SPILMOSI) | (SPIMMISO << SPILMISO));
        bits--;
        spi_traits::U1(((spi_traits::U1() & mask) | ((bits << SPILMOSI) | (bits << SPILMISO))));
    }

    static void busy_wait()
    {
        spi_traits::CMD() |= SPIBUSY;
        while(spi_traits::CMD() & SPIBUSY) {}        
    }


    // demands byte aligned copy as well
    // TODO: Make this a non-inline function to avoid code bloat
    static void write_fifo_aligned(const void* source, size_type size)
    {
        volatile uint32_t* fifoPtr = &spi_traits::W0();

        set_data_bits(size);

        const uint32_t * dataPtr = (uint32_t*) source;
        uint32_t dataSize = ((size + 3) / 4);

        while(dataSize--) {
            *fifoPtr = *dataPtr;
            dataPtr++;
            fifoPtr++;
        }

        __sync_synchronize();
        busy_wait();
    }


    // does not need to be 32-bit aligned
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
    static void write8(uint8_t value)
    {
        set_data_bits(8);
        spi_traits::W0() = value;
        busy_wait();
    }

    static void write(const void* source, size_type size)
    {
        uint8_t* s = (uint8_t*)source;
        uint32_t raw_addr = (uint32_t)s;

//#ifdef DEBUG
        int extra = raw_addr % 4;
        if(extra != 0)
        {
            // TODO: actually align these bytes
            ESP_LOGE(SPI_TAG, "Aligning bytes for write call ONLY in debug mode");
            while(extra--)
                write8(*s++);
        }
//#endif        

        write_aligned(s, size);
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