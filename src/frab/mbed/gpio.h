#pragma once

#include <mbed.h>

namespace frab {

/*
namespace gpio {

enum direction
{
    _in = 0x01,
    _out = 0x02
};

} */

struct mbed_digital_in_traits
{
    typedef int gpio_type;
    typedef DigitalIn context_t;
};

struct mbed_digital_out_traits
{
    typedef int gpio_type;
    typedef DigitalOut context_t;
};


template<>
inline int digital_in_base<mbed_digital_in_traits>::read(DigitalIn& context)
{
    return context.read();
}


template<>
inline void digital_out_base<mbed_digital_out_traits>::write(DigitalOut& context, gpio_type value)
{
    return context.write(value);
}


namespace layer2 {
/*
// NOTE: Not liking that _in | _out is gonna be a duplicate set of code
template <::frab::gpio::direction dname>
class _gpio
{
    int read();
    int write();
};

template <>
int _gpio<::frab::gpio::_in>::read()
{
    return 0;
}

template <>
int _gpio<::frab::gpio::_out>::write()
{
    return 0;
}
*/
/*
template <::frab::gpio::_in>
class _gpio
{

};*/

class digital_in : public digital_in_base<mbed_digital_in_traits>
{
    DigitalIn din;

public:
    digital_in(pin_t pin) : din(pin) {}

    gpio_type read() { return digital_in_base<mbed_digital_in_traits>::read(din); }
    void mode(gpio::digital_mode_t mode) { din.mode((PinMode)mode); }
};

class digital_out : public digital_out_base<mbed_digital_out_traits>
{
    DigitalOut dout;

public:
    digital_out(pin_t pin) : dout(pin) {}

    void write(gpio_type value) { digital_out_base<mbed_digital_out_traits>::write(dout,value ); }
};


class digital_io
{
    DigitalInOut dio;

public:
    digital_io(pin_t pin) : dio(pin) {}
};

}

// mbed-os always has a gpio_t hanging around to track
// pin, so no "true" layer1 code is possible, so we shim
// it onto layer2
namespace layer1 {

template<pin_t pin>
class digital_in : public frab::layer2::digital_in
{
public:
    digital_in() : frab::layer2::digital_in(pin) {}
};


template<pin_t pin>
class digital_out : public frab::layer2::digital_out
{
public:
    digital_out() : frab::layer2::digital_out(pin) {}
};


template<pin_t pin>
class digital_io : public frab::layer2::digital_io
{
public:
    digital_io() : frab::layer2::digital_io(pin) {}
};


}

}