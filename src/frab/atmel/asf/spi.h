#pragma once

#include <hal_spi_m_sync.h>

namespace framework_abstraction {
namespace layer1 {

template <spi_m_sync_descriptor& spi>
class SPI_sync
{
public:
    // as of this writing, not sure if blocking or not
    static void transfer(const void* in, void* out, uint16_t len)
    {
        struct spi_xfer xfer;

        xfer.rxbuf = (uint8_t*)in;
        xfer.txbuf = (uint8_t*)out;
        xfer.size = len;

        spi_m_sync_transfer(&spi, &xfer);
    }
};

}

}