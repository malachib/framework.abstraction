#pragma once

extern "C" {

#include "driver/gpio.h"

}

namespace framework_abstraction {

namespace layer0 {

namespace internal {

template <pin_t pin>
void gpio_init();

template <>
void gpio_init<15>()
{
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_GPIO15);
}

template <>
void gpio_init<4>()
{
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
}

}

// TODO: optimize with techniques from https://github.com/wdim0/esp8266_direct_gpio
template <pin_t pin>
class digital_out
{
public:
    // actually just sets pin mux for GPIO, GPIO_OUTPUT_SET sets output mode itself
    // (repeatedly)
    static void init_for_output_exp() 
    {
        internal::gpio_init<pin>();
    }

    static void write(bool value) { GPIO_OUTPUT_SET(pin, value); }
};


template <pin_t pin>
class digital_in
{
public:
    static bool read(bool value) { return GPIO_INPUT_GET(pin); }
};

}


namespace layer2 {



}

}