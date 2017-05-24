#pragma once

#include "spi_context.h"

namespace frab {

enum spi_modes
{
    SPI_MODE0 = 0,
    SPI_MODE1 = 1,
    SPI_MODE2 = 2,
    SPI_MODE3 = 3
};

template <class TSPI>
class SPI
{
    TSPI spi;
    spi_context<TSPI> context;

public:
    typedef spi_traits<TSPI> traits_t;
    typedef experimental::spi_user_context<TSPI> async_context_t;

    int transfer(uint16_t value);
    void transfer(const void* tx_buffer, size_t tx_length, void* rx_buffer, size_t rx_length);

    template <class T>
    void transfer(const T* tx_buffer, size_t tx_length, T* rx_buffer, size_t rx_length,
                  async_context_t async_context);

    void write(const void* buffer, size_t length)
    {
        transfer(buffer, length, nullptr, 0);
    }

    void write(const void* buffer, size_t length, async_context_t async_context)
    {
        transfer(buffer, length, nullptr, 0, async_context);
    }

    void read(void* buffer, size_t length)
    {
        transfer(nullptr, 0, buffer, length);
    }

    template <class ...TArgs>
    SPI(TArgs...args) : spi(args...)
    {

    }

    TSPI& native() { return spi; }

    void format(uint8_t bits, spi_modes mode = SPI_MODE0);
    void frequency(int32_t hz = 1000000);
    void begin();
    void end();
};

#ifdef __MBED__
#include "mbed/spi.hpp"
#elif defined(ARDUINO)
#include "arduino/spi.hpp"
#endif

}