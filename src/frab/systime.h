#pragma once

#include "frab.h"

// NOTE: All of this is phasing out in favor of estd::chrono namespace
#ifdef __MBED__
#include "mbed/systime.h"
#elif defined(ESP_OPEN_RTOS)
#include "esp-open-rtos/systime.h"
#elif defined(ESP_PLATFORM)
// TODO:
#elif defined(ARDUINO)
#else
//#elif __POSIX__
#include "posix/systime.h"
#endif

namespace framework_abstraction
{

uint32_t millis();

}
