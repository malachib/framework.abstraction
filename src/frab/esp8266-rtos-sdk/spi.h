#pragma once

#include "driver/spi_interface.h"
#include "driver/spi_register.h"

// NOTE: Seems very, very likely they will be changing this to a more esp-idf API at some point
// so be ready to check IDF_VER macro and dispatch accordingly

template <>
class spi_context< SpiAttr>
{

};


template <>
inline int SPI<SpiAttr>::transfer(uint16_t data)
{
    //return spi_transfer_16(spi, data);
}


template <>
inline void SPI<SpiNum>::write(const void* tx_buffer, size_t tx_length, void* rx_buffer, size_t rx_length)
{
    // dummy boilerplate.  Pausing because I never ran into formalized command/addr in SPI before
    SpiData pDat;
    pDat.cmd = cmd;			      ///< Command value
    pDat.cmdLen = 1;		      ///< Command byte length
    pDat.addr = &addr; 		      ///< Point to address value
    pDat.addrLen = 2; 	          ///< Address byte length
    pDat.data = data; 		      ///< Point to data buffer
    pDat.dataLen = sizeof(data);  ///< Data byte length.
    SPIMasterSendData(SpiNum_HSPI, &pDat);
}


namespace hal
{

typedef framework_abstraction::SPI<SpiNum> SPI;

}