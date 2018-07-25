#pragma once

#include <avr/io.h>
#include <estd/internal/platform.h>
#include <estd/internal/deduce_fixed_size.h>

// not using asf or arduino
// asf would be neat but who knows if we can use it 'offline' for 4.0
// Arduino is neat but we're trying to be a little more bare metal

namespace framework_abstraction {

struct pin
{
    enum pin_names
    {
        pa0 = 0x00,
        pa1 = 0x01,
        pa2 = 0x02,
        pa3 = 0x03,
        pa4 = 0x04,
        pa5 = 0x05,
        pa6 = 0x06,
        pa7 = 0x07,

        pb0 = 0x10,
        pb1 = 0x11,
        pb2 = 0x12,
        pb3 = 0x13,
        pb4 = 0x14,
        pb5 = 0x15,
        pb6 = 0x16,
        pb7 = 0x17,

        pc0,
        pc1,
        pc2,
        pc3,
        pc4,
        pc5,
        pc6,
        pc7,

        pd0,
        pd1,
        pd2,
        pd3,
        pd4,
        pd5,
        pd6,
        pd7
    };
};


struct ports
{
    static CONSTEXPR pin::pin_names pa = pin::pa0;
    static CONSTEXPR pin::pin_names pb = pin::pb0;
    static CONSTEXPR pin::pin_names pc = pin::pc0;
    static CONSTEXPR pin::pin_names pd = pin::pd0;
};


typedef pin::pin_names pin_type;


namespace internal {

template<bool>
struct range;

template <pin_type, typename = range<true> >
struct pin_map;

template <pin::pin_names p>
struct port
{
    typedef pin_map<p> map;

    static CONSTEXPR uint8_t base = map::base;

    void as_input(pin::pin_names pin_number)
    {
        map::ddr() &= ~(1 << (pin_number - base));
    }

    // TODO: Make this a variadic
    /*
    void as_input(pin::pin_names pin_number, pin::pin_names pin_number2)
    {
        map::ddr() &= ~(1 << (pin_number - port));
    } */

    // TODO: Make this a variadic
    void as_input(pin::pin_names pin_number, pin::pin_names pin_number2, pin::pin_names pin_number3)
    {
        map::ddr() &= ~(
            (1 << (pin_number - base)) |
            (1 << (pin_number2 - base)) |
            (1 << (pin_number3 - base))
        );
    }

    void as_output(pin::pin_names pin_number)
    {
        map::ddr() |= 1 << (pin_number - base);
    }

    void set(pin::pin_names pin_number)
    {
        map::port() |= 1 << (pin_number - base);
    }


    void clear(pin::pin_names pin_number)
    {
        map::port() &= ~(1 << (pin_number - base));
    }

    bool get(pin::pin_names pin_number)
    {
        return (map::port() & (1 << (pin_number - base))) != 0;
    }
};


template <pin_type p>
struct pin_map<p, range<(p > pin::pa0 && p <= pin::pb7)> >
{
    static volatile uint8_t& ddr() { return DDRB; }
    static volatile uint8_t& port() { return PORTB; }
    static CONSTEXPR uint8_t base = pin::pb0;
    static CONSTEXPR uint8_t pin_bit = p - pin::pb0;
};


template <pin_type p>
struct pin_map<p, range<(p > pin::pb7 && p <= pin::pc7)> >
{
    static volatile uint8_t& ddr() { return DDRC; }
    static volatile uint8_t& port() { return PORTC; }
    static CONSTEXPR uint8_t base = pin::pc0;
    static CONSTEXPR uint8_t pin_bit = p - pin::pc0;
};


template <pin_type p>
struct pin_map<p, range<(p > pin::pc7 && p <= pin::pd7)> >
{
    static volatile uint8_t& ddr() { return DDRD; }
    static volatile uint8_t& port() { return PORTD; }
    static CONSTEXPR uint8_t base = pin::pd0;
    static CONSTEXPR uint8_t pin_bit = p - pin::pd0;
};


}

namespace layer1 {

template <pin_type pin>
class digital_in
{
    typedef internal::port<pin> port;

public:
    bool read()
    {
        return port::get();
    }
};

}

}