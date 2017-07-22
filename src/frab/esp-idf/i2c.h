#pragma once

extern "C" {

#include "driver/i2c.h"
#include <stddef.h>

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

    /*
    i2c_tx_master(const i2c_tx_master&& move_from)
    {
        cmd = move_from.cmd;
    } */

    // TODO: return proper unified error code
    inline void write(uint8_t data, bool expect_ack = true)
    {
        i2c_master_write_byte(cmd, data, expect_ack); // true = ack requested
    }


    inline void write(uint8_t* data, size_t len, bool expected_ack = true)
    {
        i2c_master_write(cmd, data, len, expected_ack);
    }


    // EXPERIMENTAL
    inline void addr(uint8_t data, bool is_write_mode = true, bool expect_ack = true)
    {
        write(
            (data << 1) | (is_write_mode ? I2C_MASTER_WRITE : I2C_MASTER_READ),
            expect_ack);
    }

    inline void read(uint8_t* data, bool expect_ack = true)
    {
        i2c_master_read_byte(cmd, data, expect_ack);
    }

    inline void read(uint8_t* data, size_t len, bool expect_ack = true)
    {
        i2c_master_read(cmd, data, len, expect_ack);
    }

    // TODO: return proper unified error code
    bool commit(i2c_bus_t bus, uint32_t timeout_ms = 1000)
    {
        i2c_master_stop(cmd);
        return i2c_master_cmd_begin(bus, cmd, timeout_ms/portTICK_PERIOD_MS) == ESP_OK;
    }

/*
    // Need this 'cause we don't have in place destructors. lame
    inline void release()
    {
        i2c_cmd_link_delete(cmd);
    } */

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
    ~i2c()
    {
        i2c_driver_delete(bus);
    }

    template <bool autocommit = false>
    struct tx : public i2c_tx_master
    {
        inline bool commit()
        {
            return i2c_tx_master::commit(bus);
        }

        ~tx()
        {
            if(autocommit) commit();
        }
    };

    // FIX: decide on naming whether we want config/start/begin etc.
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

    inline tx<> get_tx()
    {
        return tx<>();
    }

    inline tx<> get_tx(uint8_t addr)
    {
        tx<> t;

        t.addr(addr);

        return t;
    }

    inline tx<true> get_tx_auto(uint8_t addr)
    {
        tx<true> t;

        // TODO: do we need to do this before each command?
        // look up i2c spec
        t.addr(addr);

        return t;
    }

    // TODO: return proper unified return code
    inline bool write(uint8_t data, bool expect_ack = true)
    {
        tx<> t;

        t.write(data, expect_ack);
        return t.commit();
    }

    // TODO: return proper unified return code
    inline bool write(uint8_t* data, size_t len, bool expect_ack = true)
    {
        tx<> t;

        t.write(data, len, expect_ack);
        return t.commit();
    }

};


}

}