#pragma once

extern "C" {

#include "espressif/esp_common.h"
#include "esp/spi.h"

}

namespace framework_abstraction {

// bus Bus ID: 0 - system, 1 - user
typedef uint8_t spi_bus_t;

inline int SPI<spi_bus_t>::transfer(uint16_t data)
{
    return spi_transfer_16(spi, data);
}


inline void SPI<spi_bus_t>::transfer(const void* tx_buffer, size_t tx_length, void* rx_buffer, size_t rx_length)
{
    // FIX: A very limited case, most definitely need to beef this up
    if(tx_length == rx_length)
        transfer(spi, tx_buffer, rx_buffer, tx_length, SPI_8BIT);
}

}
