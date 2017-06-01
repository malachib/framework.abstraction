#pragma once

extern "C" {

#include "espressif/esp_common.h"

}

namespace framework_abstraction {

struct esp_digital_in_traits
{
    typedef bool gpio_type;
    //typedef DigitalIn context_t;
    typedef pin_t context_t;
};

namespace layer1 {

// TODO: move this base class out of esp-open-rtos specific area
template <class TTraits = esp_digital_in_traits>
class digital_inout_base
{
protected:
    typedef TTraits traits_t;
    typedef typename traits_t::gpio_type gpio_type;

    // ESP SPECIFIC ONE, move this out
    inline static void direction(const pin_t pin, const gpio::direction_t direction)
    {
        gpio_enable(pin, (::gpio_direction_t)direction);
    }

    inline static void mode(const pin_t pin, const gpio::digital_mode_t mode)
    {
        if(mode == gpio::open_drain)
        {
            gpio_enable(pin, ::gpio_direction_t::GPIO_OUT_OPEN_DRAIN);
        }
        else if(mode == gpio::pull_up)
        {
            // false for 'during sleep' mode, which docs say is untested
            gpio_set_pullup(pin, true, false);
        }
    }

    inline static void mode(const pin_t pin, const gpio::direction_t d, const gpio::digital_mode_t m = -1)
    {
        direction(pin, d);
        mode(pin, m);
    }
};


template<pin_t pin>
class digital_in : public digital_inout_base<>
{
public:
    inline gpio_type read() { return gpio_read(pin); }
};

template<pin_t pin>
class digital_out : public digital_inout_base<>
{
public:
    digital_out() { direction(pin, gpio::output); }

    inline void write(const gpio_type set) { gpio_write(pin, set); }
};

}

namespace layer2 {

// TODO: move this base class out of esp-open-rtos specific area
template <class TTraits = esp_digital_in_traits>
class digital_inout_base : layer1::digital_inout_base<TTraits>
{
protected:
    typedef TTraits traits_t;
    typedef typename traits_t::gpio_type gpio_type;
    typedef typename traits_t::context_t context_t;

    context_t context;

    /*
    // ESP SPECIFIC ONE, move this out
    inline static void direction(pin_t pin, const gpio::direction_t direction)
    {
        gpio_enable(pin, (::gpio_direction_t)direction);
    }

    inline static void mode(pin_t pin, const gpio::digital_mode_t mode)
    {
        if(mode == gpio::open_drain)
        {
            gpio_enable(pin, ::gpio_direction_t::GPIO_OUT_OPEN_DRAIN);
        }
        else if(mode == gpio::pull_up)
        {
            // false for 'during sleep' mode, which docs say is untested
            gpio_set_pullup(pin, true, false);
        }
    } */

    digital_inout_base(context_t context) : context(context) {}
};

class digital_in : public digital_inout_base<>
{
public:
    digital_in(pin_t pin) : digital_inout_base(pin) {}

    inline gpio_type read() { return gpio_read(context); }
};

class digital_out : public digital_inout_base<>
{
public:
    digital_out(pin_t pin) : digital_inout_base<>(pin) {}

    inline void write(const gpio_type value) { gpio_write(context, value); }
};

}


}