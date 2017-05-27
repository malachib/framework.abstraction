#pragma once

#include <Arduino.h>

namespace framework_abstraction {

inline void yield()
{
    ::yield();
}

}