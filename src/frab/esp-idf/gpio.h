#pragma once

extern "C" {
#include "driver/gpio.h"
}

namespace framework_abstraction {


namespace layer1 {


template<>
inline value_t digital_in_base_new<>::read(context_t &context)
{
    return gpio_get_level(context);
}


template<>
inline void digital_out_base_new<>::write(context_t &context, value_t value)
{
    gpio_set_level(context, value);
}

template<gpio_num_t pin, bool open_drain = false>
struct digital_in : public digital_in_base_new<pin>
{
public:
    digital_in()
    {
        gpio_set_direction(pin, GPIO_MODE_DEF_INPUT | (open_drain ? GPIO_MODE_DEF_OD | 0));
    }
};


template<gpio_num_t pin>
struct digital_out : public digital_out_base_new<pin>
{
    digital_in()
    {
        gpio_set_direction(pin, GPIO_MODE_DEF_OUTPUT);
    }
};


template<gpio_num_t pin, bool open_drain = false>
class digital_io :
    public digital_in_base_new<pin>,
    public digital_out_base_new<pin>
{
public:
    digital_io()
    {
        gpio_set_direction(pin, GPIO_MODE_INPUT_OUTPUT | (open_drain ? GPIO_MODE_DEF_OD | 0));
    }
};


}

}