#pragma once

#include <Arduino.h>

namespace framework_abstraction {

uint32_t millis() { return ::millis(); }

}