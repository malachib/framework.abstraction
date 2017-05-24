#pragma once

#include <mbed.h>

namespace frab {

// NOTE: this rolls over every 30 mins cuz it comes from us
// but just to get things going we'll use it
inline uint32_t millis()
{
    return us_ticker_read() / 1000;
}

}