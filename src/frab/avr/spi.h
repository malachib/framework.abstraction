#pragma once

#include "gpio.h"

namespace framework_abstraction {

// using preprocessor defs from https://www.microchip.com/webdoc/AVRLibcReferenceManual/using_tools_1using_avr_gcc_mach_opt.html
struct spi_pins
{
    static CONSTEXPR pin_type port = pin::pb0;
    
// atmega328p spi pins are:
// PB5 = SCK, PB4 = MISO, PB3 = MOSI, PB2 = SS
#if defined(__AVR_ATmega328P__)
    enum pin_names
    {
        sck = pin::pb5,
        miso = pin::pb4,
        mosi = pin::pb3,
        cs = pin::pb2
    };
#elif defined(__AVR_ATmega32U4__)
    static CONSTEXPR pin_type sck = pin::pb1;
    static CONSTEXPR pin_type miso = pin::pb3;
    static CONSTEXPR pin_type mosi = pin::pb2;
    static CONSTEXPR pin_type cs = pin::pb0;
#elif defined(__AVR_ATtiny85__)
    static CONSTEXPR pin_type sck = pin::pb2;
    static CONSTEXPR pin_type miso = pin::pb1;
    static CONSTEXPR pin_type mosi = pin::pb0;
    static CONSTEXPR pin_type cs = pin::pb3;
#endif
};

}