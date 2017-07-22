#pragma once

extern "C" {

// using extras/i2c since esp8266 doesn't actually have built-in i2c
#include <i2c/i2c.h>

}
// NOTE: 100% untested!

namespace framework_abstraction {

namespace layer1 {

class i2c_tx_auto
{
public:
    i2c_tx_auto()
    {
        i2c_start();
    }

    ~i2c_tx_auto()
    {
        i2c_stop();
    }

    inline void write(uint8_t value)
    {
        i2c_write(value);
    }

    /*
     this one sends out addr too which is not compatible with esp-idf flavor
     see if we can leverage it somehow
    inline void write(const uint8_t* value, size_t len)
    {
        i2c_slave_write(addr, value, len);
    } */

    inline void write(const uint8_t* value, size_t len)
    {
        while(len--)
            i2c_write(*value++);
    }

    // EXPERIMENTAL
    inline void addr(uint8_t data, bool is_write_mode = true)
    {
        write(
            (data << 1) | (is_write_mode ? I2C_MASTER_WRITE : I2C_MASTER_READ));
    }
};

template <uint8_t pin_sda, uint8_t pin_scl>
class i2c
{
public:
    i2c_tx_auto get_tx_auto()
    {
        return i2c_tx_auto();
    }
}

}
}