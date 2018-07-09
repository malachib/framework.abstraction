#pragma once

#include "driver/spi_interface.h"
#include "driver/spi_register.h"

// NOTE: Seems very, very likely they will be changing this to a more esp-idf API at some point
// so be ready to check IDF_VER macro and dispatch accordingly

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