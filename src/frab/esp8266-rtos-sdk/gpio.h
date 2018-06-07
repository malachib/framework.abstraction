#pragma once

extern "C" {

#include "driver/gpio.h"

}

namespace framework_abstraction {

namespace layer1 {

template <pin_t pin>
class digital_out
{
public:
    static void write(bool value) { GPIO_OUTPUT_SET(pin, value); }
};


template <pin_t pin>
class digital_in
{
public:
    static void read(bool value) { GPIO_INPUT_GET(pin); }
};

}

}