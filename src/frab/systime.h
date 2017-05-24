#pragma once

#include "frab.h"

#ifdef __MBED__
#include "mbed/systime.h"
#else
//#elif __POSIX__
#include "posix/systime.h"
#endif

namespace frab
{

uint32_t millis();

}