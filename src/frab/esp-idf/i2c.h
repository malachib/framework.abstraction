#pragma once

extern "C" {

#include "driver/i2c.h"
}

namespace framework_abstraction {

typedef i2c_port_t i2c_bus_t;

struct i2c_config_t
{
    // FIX: master mode code
    uint32_t clock_speed;
};

namespace layer1 {

class i2c_tx_master
{
    i2c_cmd_handle_t cmd;

public:
    // TODO: scope this to only be accessible by our i2c classes
    i2c_tx_master()
    {
        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
    }

    i2c_tx_master(const i2c_tx_master&& move_from)
    {
        cmd = move_from.cmd;
    }

    void write(uint8_t data)
    {
        i2c_master_write_byte(cmd, data, true); // true = ack requested
    }

    void commit(i2c_bus_t bus)
    {
        i2c_master_stop(cmd);
        i2c_master_cmd_begin(bus, cmd, 1000/portTICK_PERIOD_MS);
    }

    ~i2c_tx_master()
    {
        i2c_cmd_link_delete(cmd);
    }
};

// FIX: this is all hardwired to i2c master
template <i2c_bus_t bus, gpio_num_t pin_sda, gpio_num_t pin_sdl>
class i2c
{
protected:
    void config(::i2c_config_t& i2c_config)
    {
        i2c_param_config(bus, &i2c_config);
    }

public:
    void config(uint32_t clock_speed)
    {
        ::i2c_config_t native_config;

        native_config.mode = I2C_MODE_MASTER;
        native_config.sda_io_num = pin_sda;
        native_config.scl_io_num = pin_sdl;
        native_config.sda_pullup_en = GPIO_PULLUP_ENABLE;
        native_config.scl_pullup_en = GPIO_PULLUP_ENABLE;
        native_config.master.clk_speed = clock_speed;

        config(native_config);
        i2c_driver_install(bus, I2C_MODE_MASTER, 0, 0, 0);
    }

    i2c_tx_master tx_begin_experimental()
    {
        return i2c_tx_master();
    }

    void write(uint8_t data)
    {
        i2c_tx_master tx;

        tx.write(data);
        tx.commit(bus);
    }

};


}

}