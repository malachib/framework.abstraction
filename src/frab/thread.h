#pragma once

#ifdef __MBED__
#include "mbed/thread.h"
#elif defined(ARDUINO)
#include "arduino/thread.h"
#else
namespace framework_abstraction {
inline void yield() {}
}
#endif