#pragma once

#include <stdint.h>
#include <new>

namespace framework_abstraction {

namespace experimental {

// TODO: pass in "release function"
template <class T>
class placement_helper
{
    uint8_t buffer[sizeof(T)];

public:
    inline T& get() const
    {
        return * reinterpret_cast<T*>((uint8_t*)buffer);
    }

    T& construct()
    {
        //T* existing = reinterpret_cast<T>(buffer);
        T* value = new(buffer) T();
        return *value;
    }

    void destruct()
    {
        // Have to do this because C++ doesn't have a placement delete
        //get().release();
        get().~T();
    }
};

}
    

}