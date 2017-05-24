#pragma once

#ifdef __MBED__
#include <mbed.h>
#endif

namespace frab {

struct gpio
{
    enum digital_state_t
    {
        low = 0,
        high = 1
    };

#ifdef __MBED__
    // shamelessly lifted from mbed
    enum digital_mode_t
    {
        pull_up = PinMode::PullUp,
        pull_down = PinMode::PullDown,
        pull_none = PinMode::PullNone,
        open_drain = PinMode::OpenDrain
    };
#else
#endif
};

#ifdef __MBED__
typedef PinName pin_t;
#else
typedef int pin_t;
#endif

template <class TTraits>
class digital_in_base
{
protected:
    typedef TTraits traits_t;
    typedef typename traits_t::gpio_type gpio_type;

public:
    static gpio_type read(typename traits_t::context_t& context);
};

template <class TTraits>
class digital_out_base
{
protected:
    typedef TTraits traits_t;
    typedef typename traits_t::gpio_type gpio_type;

public:
    gpio_type write();
    static void write(typename traits_t::context_t& context, gpio_type value);
};

namespace layer1 {

/*
template<pin_t pin>
class gpio;
*/
}


namespace layer2 {
/*
class gpio
{
public:
};
*/
}

namespace layer5 {

template <class TTraits>
class digital_in_base : public frab::digital_in_base<TTraits>
{
protected:
    typedef TTraits traits_t;
    typedef typename traits_t::gpio_type gpio_type;

public:
    virtual gpio_type read() = 0;
};

template <class TTraits>
class digital_out_base : public frab::digital_out_base<TTraits>
{
protected:
    typedef TTraits traits_t;
    typedef typename traits_t::gpio_type gpio_type;

public:
    virtual void write(gpio_type value) = 0;
};

}

}

#ifdef __MBED__
#include "mbed/gpio.h"
#endif