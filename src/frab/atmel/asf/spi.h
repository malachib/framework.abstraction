#pragma once

// NOTE: As ASF is modularly generated, not all headers are gaurunteed available
// maybe we can detect this with clever use of #include_next
#include <hal_spi_m_sync.h>
#include <hal_spi_m_async.h>

#include "gpio.h"

namespace framework_abstraction {
namespace layer0 {

// synchronous (blocking) master SPI
// cs_pin is an experimental feature.  Some frameworks manage the CS pin for you, some 
// do not
#ifdef _HAL_SPI_M_SYNC_H_INCLUDED
template <spi_m_sync_descriptor& spi, pin_t cs_pin = -1>
class SPI_sync
{
public:
    // "Perform the SPI data transfer (TX and RX) in polling way." - asf docs
    // also
    // "Activate CS, do TX and RX and deactivate CS. It blocks" which is unclear
    // whether caller or system does the CS.  However, my own testing and inspection
    // of spi_m_sync_descriptor indicates this is up to the caller
    static uint32_t transfer(const void* in, void* out, uint16_t len)
    {
        struct spi_xfer xfer;

        xfer.rxbuf = (uint8_t*)in;
        xfer.txbuf = (uint8_t*)out;
        xfer.size = len;

        // experimental, enable cs if set up.  No provision for
        // delays etc
        if(cs_pin != -1) digital_out<cs_pin, false>::level(false);

        uint32_t result = spi_m_sync_transfer(&spi, &xfer);

        if(cs_pin != -1) digital_out<cs_pin, false>::level(true);

        return result;
    }
};
#endif

// asynchronous (nonblocking) master SPI
#ifdef _HAL_SPI_M_ASYNC_H_INCLUDED
template <spi_m_async_descriptor& spi>
class SPI_async
{
public:
};
#endif

}

}