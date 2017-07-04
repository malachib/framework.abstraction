#pragma once

#include "frab.h"

#ifdef __MBED__
#include "mbed/systime.h"
#elif defined(ESP_OPEN_RTOS)
#include "esp-open-rtos/systime.h"
#elif defined(ESP_PLATFORM)
// TODO:
#else
//#elif __POSIX__
#include "posix/systime.h"
#endif

namespace framework_abstraction
{

uint32_t millis();

}
