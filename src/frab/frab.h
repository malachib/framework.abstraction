#pragma once

#include <stdint.h>
#include <new>

namespace framework_abstraction {

namespace experimental {

#define EXPERIMENTAL_PLACEMENT_RECYCLE 

// TODO: pass in "release function"
template <class T>
class placement_helper
{
    uint8_t buffer[sizeof(T)];

    // Adapted from https://stackoverflow.com/questions/13786888/check-if-member-exists-using-enable-if
    struct general_ {};
    struct special_ : general_ {};
    template<typename> struct int_ { typedef int type; };

    // FIX: this isn't working, this recycle helper never gets called
    // if explicit recycle method exists, call it
    template<class _T, typename int_<decltype(_T::recycle())>::type = 0>
    bool recycle_helper(_T&& ignored, special_)
    {
        get().recycle();
        return true;
    }

    // otherwise, do a standard destroy/construct phase (which may require
    // constructor arguments)
    template <class _T, class ...TArgs>
    bool recycle_helper(_T&& ignored, general_, TArgs...args)
    {
        destroy();
        construct(args...);
        return false;
    }

    /* Doesn't help make special_ flavor of recycle get called
    template <class _T>
    bool recycle_helper(_T&& ignored, general_)
    {
        destroy();
        construct();
        return false;
    } */

public:
    inline T& get() const
    {
        return * reinterpret_cast<T*>((uint8_t*)buffer);
    }

    operator T&() const { return get(); }

    template <class ...TArgs>
    T& construct(TArgs...args)
    {
        //T* existing = reinterpret_cast<T>(buffer);
        T* value = new(buffer) T(args...);
        return *value;
    }

    void destroy()
    {
        // Have to do this because C++ doesn't have a placement delete
        //get().release();
        get().~T();
    }

    // DEPRECATED
    void destruct() { destroy(); }

    template <class ...TArgs>
    T& recycle(TArgs...args)
    {
#ifdef EXPERIMENTAL_PLACEMENT_RECYCLE
        recycle_helper(get(), special_(), args...);
        return get();
#else
        destroy();
        return construct(args...);
#endif
    }
};

}
    

}