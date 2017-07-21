#pragma once

extern "C" {
#include <stddef.h>
}

namespace framework_abstraction {

typedef uint16_t i2c_bus_t;

struct i2c_config_t
{
    // FIX: master mode code
    uint32_t clock_speed;
};

namespace layer1 {

class i2c
{
public:
    struct _tx 
    {
        void write(uint8_t data, bool expect_ack = true) {}
        void write(const uint8_t* data, size_t len, bool expect_ack = true) {}
    };

    void config(int) {}

    _tx get_tx_auto(uint8_t addr) 
    { 
        return _tx(); 
    }
};

}
}