#pragma once

namespace framework_abstraction {

// container for i2c constants
struct i2c_experimental
{
    enum acknowledge_code_t
    {
        ack,
        nack,
        timeout // only used during write operations
    };
};

}

#ifdef ESP_PLATFORM
#include "esp-idf/i2c.h"
#elif __MBED__
#include "mbed/i2c.h"
#else
#error "Unspported platform"
#endif