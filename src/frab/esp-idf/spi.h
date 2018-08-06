#pragma once

#include <estd/internal/platform.h>
#include <estd/queue.h>

#include "driver/spi_common.h"
#include "driver/spi_master.h"

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
template <spi_host_device_t host, class TPolicy = spi_policy>
class SPI
{
    typedef typename TPolicy::tx_queue_type tx_queue_type;
    tx_queue_type transactions;
    spi_device_handle_t device;

    spi_transaction_t& back() { return transactions.back(); }

public:
    void set_command(uint16_t cmd, uint8_t command_bits );

    void add_device(spi_device_interface_config_t& config)
    {
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
        spi_device_queue_trans(device, &transactions.front(), portMAX_DELAY);
    }


    // blocking version, do not use when using non blocking (async) version
    void transfer(void* data, uint16_t len)
    {
        spi_transaction_t t;
        memset(&t, 0, sizeof(t));
        t.tx_buffer = data;
        t.rx_buffer = data;
        t.length = 8*len;
        esp_err_t ret = spi_device_transmit(device, &t);
    }
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

}