#pragma once

#include <Thread.h>

namespace framework_abstraction {

inline void yield()
{
    Thread::yield();
}

}