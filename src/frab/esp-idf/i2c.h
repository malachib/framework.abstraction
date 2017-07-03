#pragma once

extern "C" {

#include "driver/i2c.h"
}

namespace framework_abstraction {

typedef uint8_t i2c_bus_t;

namespace layer1 {

template <i2c_bus_t bus>
class i2c
{
protected:
    void config(i2c_config_t& i2c_config)
    {
        i2c_param_config(bus, &i2c_config);
    }

public:

};


}

}