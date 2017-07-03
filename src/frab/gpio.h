#pragma once

#ifdef __MBED__
#include <mbed.h>
#endif

namespace framework_abstraction {

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
#elif defined(ESP_OPEN_RTOS)
    enum digital_mode_t
    {
        pull_up,
        open_drain
    };

    enum direction_t
    {
        input = ::gpio_direction_t::GPIO_INPUT,
        output = ::gpio_direction_t::GPIO_OUTPUT
    };
#elif defined(ESP_PLATFORM)
    enum direction_t
    {
        input = GPIO_MODE_DEF_INPUT,
        output = GPIO_MODE_DEF_OUTPUT
    };

    enum digital_mode_t
    {
        pull_up,
        pull_down,
        open_drain
    };
#else
#endif
};

#ifdef __MBED__
typedef PinName pin_t;

struct gpio_traits
{
    typedef PinName pin_t;
    typedef DigitalIn   context_in_t;
    typedef DigitalOut  context_out_t;
    typedef int value_t;
};


#elif ESP_PLATFORM
typedef gpio_num_t pin_t;

struct gpio_traits
{
    typedef gpio_num_t pin_t;
    typedef pin_t context_in_t;
    typedef pin_t context_out_t;
    typedef int value_t;
};

#else
typedef int pin_t;

struct gpio_traits
{
    typedef int pin_t;
    typedef pin_t context_in_t;
    typedef pin_t context_out_t;
    typedef int value_t;
};
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

// "easy" version when pin_t and context_in_t and context_out_t all map to the
// same thing
template<class TTraits = gpio_traits>
class digital_in_base_new //: public framework_abstraction::digital_in_base<TTraits>
{
    typedef typename TTraits::context_in_t context_t;
    typedef typename TTraits::value_t value_t;

protected:
    static value_t read(context_t& context);
};


template<class TTraits = gpio_traits>
class digital_out_base_new //: public framework_abstraction::digital_out_base<TTraits>
{
    typedef typename TTraits::context_out_t context_t;
    typedef typename TTraits::value_t value_t;

protected:
    static void write(context_t& context, value_t value);
};


namespace layer1 {

// "easy" version when pin_t and context_in_t and context_out_t all map to the
// same thing
template<pin_t pin, class TTraits = gpio_traits>
class digital_in_base : public framework_abstraction::digital_in_base_new<TTraits>
{
    typedef framework_abstraction::digital_in_base_new<TTraits> base_t;
    typedef typename TTraits::context_in_t context_t;
    typedef typename TTraits::value_t value_t;

public:
    inline value_t read() { base_t::read(pin); }
};


template<pin_t pin, class TTraits = gpio_traits>
class digital_out_base : public framework_abstraction::digital_out_base_new<TTraits>
{
    typedef framework_abstraction::digital_out_base_new <TTraits> base_t;
    typedef typename TTraits::context_out_t context_t;
    typedef typename TTraits::value_t value_t;

public:
    inline void write(value_t value) { base_t::write(pin, value); }
};


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
class digital_in_base : public framework_abstraction::digital_in_base<TTraits>
{
protected:
    typedef TTraits traits_t;
    typedef typename traits_t::gpio_type gpio_type;

public:
    virtual gpio_type read() = 0;
};

template <class TTraits>
class digital_out_base : public framework_abstraction::digital_out_base<TTraits>
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
#elif defined(ESP_OPEN_RTOS)
#include "esp-open-rtos/gpio.h"
#elif defined(ARDUINO)
//#include "arduino/gpio.h"
#endif