#pragma once

#include <stdint.h>
#include <hal_gpio.h>

namespace framework_abstraction {

typedef uint8_t pin_t;
typedef bool gpio_type;

namespace layer0 {

template<pin_t pin>
class digital_in
{
public:
    inline static gpio_type level()
    { 
        return gpio_get_pin_level(pin); 
    }

    inline static gpio_type read() { return level(); }
};

template<pin_t pin>
class digital_out
{
public:
    //digital_out() { direction(pin, gpio::output); }

    inline static void level(const gpio_type set)
    {
        gpio_set_pin_level(pin, set); 
    }

    inline static void write(const gpio_type set) { level(set); }
};


}

namespace layer1 {

}

}
