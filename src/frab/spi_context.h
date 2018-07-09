#pragma once

// works for mbed os *and* Arduino
#if defined(__MBED__) || defined(ARDUINO)
#include <SPI.h>
#endif

#if DEVICE_SPI_ASYNCH
#include <platform/Callback.h>
#endif

namespace framework_abstraction
{

template <class TSPI>
class spi_context;


namespace experimental
{

template <class TSPI>
class spi_base;

// Platform specific context to push into transfer function
// unlike most of our classes, this one is not opqaue, calling code
// must populate its contents
template <class TSPI>
class spi_user_context;


}

template <class TSPI>
class spi_traits
{
public:
    static bool can_set_bitwidth();
    static bool can_set_dataorder();

    static bool async(); // does this SPI devie support async API
};


// TODO: Specialization is such we can move this out into the specific platform areas, don't need this
// 2nd spi_context include
#ifdef __MBED__

template <>
class spi_context<mbed::SPI>
{

};

namespace experimental
{

template <>
class spi_user_context<mbed::SPI>
{
public:
    mbed::event_callback_t& callback;
    const int event;

    spi_user_context(mbed::event_callback_t& callback, int event = SPI_EVENT_COMPLETE) :
        callback(callback),
        event(event)
        {}
};

}

template <>
class spi_traits<mbed::SPI>
{
public:
    static bool can_set_bitwidth() { return true; }
    static bool can_set_dataorder() { return false; }

    static bool async() { return DEVICE_SPI_ASYNCH; }
};


#elif defined(ARDUINO)

// UNTESTED CODE
template <>
class spi_context< ::SPI> : public SPISettings
{

};

template <>
class spi_traits< ::SPI>
{
public:
    static bool can_set_bitwidth() { return true; }
    static bool can_set_dataorder() { return true; }

    static bool async() { return false; }
};


#endif

}