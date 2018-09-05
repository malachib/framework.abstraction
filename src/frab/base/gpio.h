#pragma once

namespace framework_abstraction {

// "easy" version when pin_t and context_in_t and context_out_t all map to the
// same thing
template<class TTraits>
class digital_in_base_new //: public framework_abstraction::digital_in_base<TTraits>
{
    typedef typename TTraits::context_in_t context_t;
    typedef const context_t& context_reference;
    typedef typename TTraits::value_t value_t;

protected:
    static value_t read(context_reference context);
};


template<class TTraits>
class digital_out_base_new //: public framework_abstraction::digital_out_base<TTraits>
{
    typedef typename TTraits::context_out_t context_t;
    typedef const context_t& context_reference;
    typedef typename TTraits::value_t value_t;

protected:
    static void write(context_reference context, value_t value);
};



namespace layer0 {

// "easy" version when pin_t and context_in_t and context_out_t all map to the
// same thing
template<class TTraits, typename TTraits::pin_t pin>
class digital_in_base_v2 : public framework_abstraction::digital_in_base_new<TTraits>
{
    typedef framework_abstraction::digital_in_base_new<TTraits> base_t;
    typedef typename TTraits::context_in_t context_t;
    typedef typename TTraits::value_t value_t;

public:
    static inline value_t read() { base_t::read(pin); }
    static inline value_t level() { return read(); }

    operator value_t() const { return level(); }
};


template<class TTraits, typename TTraits::pin_t pin>
class digital_out_base_v2 : public framework_abstraction::digital_out_base_new<TTraits>
{
    typedef framework_abstraction::digital_out_base_new <TTraits> base_t;
    typedef typename TTraits::context_out_t context_t;
    typedef typename TTraits::value_t value_t;

public:
    static inline void write(value_t value) { base_t::write(pin, value); }
    static inline void level(value_t value) { write(value); }

    digital_out_base_v2& operator=(const value_t set)
    {
        write(set);
        return *this;
    }
};

}

}