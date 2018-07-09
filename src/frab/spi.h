#pragma once

#include "spi_context.h"
#include <stdint.h> // for uint32_t and friends
#include <stdlib.h> // for size_t

namespace framework_abstraction {

enum spi_modes
{
    SPI_MODE0 = 0,
    SPI_MODE1 = 1,
    SPI_MODE2 = 2,
    SPI_MODE3 = 3
};

typedef spi_modes spi_mode_t;

typedef enum _spi_endianness_t {
    SPI_LITTLE_ENDIAN = 0,
    SPI_BIG_ENDIAN
} spi_endianness_t;

/**
 * SPI bus settings
 * (lifted directly from ESP-OPEN-RTOS), needs attention/adjustment.  Not used yet
 */
struct
{
    spi_mode_t mode;              ///< Bus mode
    uint32_t freq_divider;        ///< Bus frequency as a divider. See spi_init()
    bool msb;                     ///< MSB first if true
    spi_endianness_t endianness;  ///< Bus byte order
    bool minimal_pins;            ///< Minimal set of pins if true. Spee spi_init()
} spi_settings_t;

// TODO: implicitly a layer 2 class, break it out appropriately
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

    // perhaps instead of begin/end we can synchronize periodically.  Note
    // the whole queuing notion is a bit on its head if doing 'transfer' exclusively
    void sync_experimental();
};

#ifdef __MBED__
#include "mbed/spi.hpp"
#elif defined(ARDUINO)
#include "arduino/spi.hpp"
#elif defined(ESP_OPEN_RTOS)
#include "esp-open-rtos/spi.h"
#elif defined(ESP_PLATFORM) && defined(ESP8266)
#include "esp8266-rtos-sdk/spi.h"
#else
#error Unsupported Platform
//#elif defined(ESP_PLATFORM) && defined(ESP32)
#endif

}
