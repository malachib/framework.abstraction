#pragma once

#include <time.h>

namespace framework_abstraction {


inline uint32_t millis()
{
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    return spec.tv_nsec / 1000;
}


}