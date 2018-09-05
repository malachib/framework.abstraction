#pragma once

#include <estd/internal/platform.h>
#include <estd/queue.h>

#include "driver/spi_common.h"
#include "driver/spi_master.h"

#include "../base/spi.h"

namespace framework_abstraction {

namespace driver {

// ESP32 esp-idf has notion of 
// 'spi bus' for configuring pins, 
//    max xfer size (odd because we're expected to manually allocate DMA memory)
//    i guess this is for sanity check? it doesn't seem to allocate on its own
// 'spi device' for configuring SPI mode, speed, cs pin

struct spi_policy
{
    typedef estd::layer1::queue<spi_transaction_t, 4> tx_queue_type;
};


// TODO: break out async and blocking API so that someone can make a 
// version which doesn't demand a transacations queue sitting around
// NOTE: This is actually shaping up to be a 'spi device' class, in which
// case 'host' template parameter will go away
template <class TPolicy = spi_policy>
class SPI_device : 
    protected TPolicy
{
    typedef TPolicy policy_base;
    typedef typename TPolicy::tx_queue_type tx_queue_type;
    tx_queue_type _transactions;
    tx_queue_type& transactions() { return _transactions; }
    spi_device_handle_t device;

    spi_transaction_t& back() { return transactions().back(); }

    // proof of concept for interaction with embr subject/observer
    // code
    template <class TNotify>
    static void do_notify(const TNotify&)
    {
        // TODO: put subject.do_notify here
    }

    static void pre_cb(spi_transaction_t* t)
    {
        event_tx_begin_experimental e;

        e.context = t->user;

        do_notify(e);
    }


    static void post_cb(spi_transaction_t* t)
    {
        event_tx_end_experimental e;

        e.context = t->user;

        do_notify(e);
    }

public:
    void set_command(uint16_t cmd, uint8_t command_bits );

    void add_device(spi_host_device_t host, spi_device_interface_config_t& config, bool set_exp_callbacks = true)
    {
        if(set_exp_callbacks)
        {
            config.post_cb = &post_cb;
            config.pre_cb = &pre_cb;
        }

        spi_bus_add_device(host, &config, &device);
    }

    uint8_t* preallocated_tx_area()
    {
        return back().tx_data();
    }

    // dma + tx oriented
    void set_write_buffer(const void* buffer)
    {
        back().tx_buffer = buffer;
    }

    void set_read_buffer(void* buffer)
    {
        back().rx_buffer = buffer;
    }

    void enqueue()
    {
        spi_device_queue_trans(device, &transactions().front(), portMAX_DELAY);
    }


    // blocking version, do not use when using non blocking (async) version
    // among other things, spi_transaction_t does not last past the scope
    // of this call
    void transfer(const void* in, void* out, uint16_t len)
    {
        spi_transaction_t t;
        memset(&t, 0, sizeof(t));
        t.tx_buffer = out;
        t.rx_buffer = in;
        t.length = 8*len;
        esp_err_t ret = spi_device_transmit(device, &t);
    }


    // totally untested
    uint8_t transfer8(uint8_t value)
    {
        // Usage of SPI_USE_TXDATA is ambiguous.  They only imply
        // that the flag is set with t.flags
        spi_transaction_t t;

        memset(&t, 0, sizeof(t));

        t.flags |= SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA;
        t.tx_data[0] = value;
        t.length = 8; // 8 bits

        esp_err_t ret = spi_device_transmit(device, &t);

        return t.rx_data[0];
    }


    // totally untested
    // if it works, consider optimizing by dumping rx phase
    void write8(uint8_t value)
    {
        transfer8(value);
    }

    operator spi_device_handle_t() const { return device; }
};


template <spi_host_device_t host>
struct SPI_bus
{
    static void initialize(spi_bus_config_t& config, int dma_chan)
    {
        spi_bus_initialize(host, &config, dma_chan);
    }

    template <class TPolicy>
    static void add(SPI_device<TPolicy>& d, spi_device_interface_config_t& config)
    {
        d.add_device(host, config);
    }

    operator spi_host_device_t() const { return host; }
};




struct spi_traits
{
    static CONSTEXPR bool has_dma = true;

    static CONSTEXPR bool has_fifo = false;
    static CONSTEXPR bool has_async = true;
    static CONSTEXPR bool has_blocking = true;
    static CONSTEXPR bool has_cmd_phase = true;
    static CONSTEXPR bool has_addr_phase = true;
    static CONSTEXPR bool has_full_duplex = true;
    static CONSTEXPR bool has_half_duplex = true;
    static CONSTEXPR bool has_transaction = true;

    static CONSTEXPR bool has_preallocated_tx_area = true;
    static CONSTEXPR bool has_preallocated_rx_area = true;
    
    static CONSTEXPR int tx_area_size = 4;
    static CONSTEXPR int rx_area_size = 4;

    struct event
    {
        static CONSTEXPR bool has_sending = true;
        static CONSTEXPR bool has_sent = true;
        typedef spi_transaction_t* sending_parameter;
        typedef spi_transaction_t* sent_parameter;
        static void* context(sending_parameter p) { return p->user; }
    };
};

}

/*
 * This is more of a layer1 thing, but the 'host' hardwiring is layer0
 * so deliberate a bit more before committing.  Remember though, 'host'
 * itself is barely used and could be relegated to distinct API calls
namespace layer0 {

template <spi_host_device_t host, class TPolicy = driver::spi_policy>
using SPI = driver::SPI<host, TPolicy>;

} */

}