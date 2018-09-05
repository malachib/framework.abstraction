#pragma once

#include <stdint.h>
#include <hal_gpio.h>

#include "../../base/gpio.h"

namespace framework_abstraction {

#ifndef FRAB_EXPERIMENTAL_GLOBAL_GPIO
typedef uint8_t pin_t;
#endif
typedef bool gpio_type;

namespace layer0 {

// init set to false only during testing - eventually we'd like these to be true
// *except* that ASF leans towards initializing these for you
template<pin_t pin, bool init = false>
class digital_in
{
public:
    inline static gpio_type level()
    { 
        return gpio_get_pin_level(pin); 
    }

    inline static gpio_type read() { return level(); }

    operator bool() const { return level(); }
};

template<pin_t pin, bool init = false>
class digital_out
{
public:
    //digital_out() { direction(pin, gpio::output); }
    digital_out()
    {
        if(init)
        {
            gpio_set_pin_direction(pin, GPIO_DIRECTION_OUT);
        }
    }

    inline static void level(const gpio_type set)
    {
        gpio_set_pin_level(pin, set); 
    }

    inline static void write(const gpio_type set) { level(set); }

    digital_out& operator=(const gpio_type set)
    {
        write(set);
        return *this;
    }
};


}

namespace layer1 {

}

}
