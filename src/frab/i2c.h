#pragma once

#ifdef ESP_PLATFORM
#include "esp-idf/i2c.h"
#else
#error "Unspported platform"
#endif