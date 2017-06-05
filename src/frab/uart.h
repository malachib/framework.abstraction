#pragma once

namespace framework_abstraction {

// platform specific uart settings type, specifically
// which pins/port to address.  BPS rate, stop bits, 
// and other stuff is done elsewhere
typedef int uart_settings_t;

namespace layer1 {

template <uart_settings_t uart_settings>
class uart
{
public:
    void bps(uint32_t bps);
    void stopbits(uint8_t stopbits);

    // FIX: need to align methodology with SPI and I2C for async
    // interaction
    size_t read_async(uint8_t* buffer, size_t maxlen);
    size_t read(uint8_t* buffer, size_t maxlen);

    size_t write_async(const uint8_t* buffer, size_t maxlen);
    size_t write(const uint8_t* buffer, size_t maxlen);
    
    int get();
    void put(uint8_t value);
    
    size_t available();
};

}

namespace layer2 {

class uart
{
    uart_settings_t settings;
    
public:
    void bps(uint32_t bps);
    void stopbits(uint8_t stopbits);

    // FIX: need to align methodology with SPI and I2C for async
    // interaction
    size_t read_async(uint8_t* buffer, size_t maxlen);
    size_t read(uint8_t* buffer, size_t maxlen);

    size_t write_async(const uint8_t* buffer, size_t maxlen);
    size_t write(const uint8_t* buffer, size_t maxlen);
    
    int get();
    void put(uint8_t value);

    size_t available();
};

}


namespace layer5 {

class uart_base
{
    uart_settings_t settings;
    
public:
    virtual void bps(uint32_t bps);
    virtual void stopbits(uint8_t stopbits);

    // FIX: need to align methodology with SPI and I2C for async
    // interaction
    virtual size_t read_async(uint8_t* buffer, size_t maxlen);
    virtual size_t read(uint8_t* buffer, size_t maxlen);

    virtual size_t write_async(const uint8_t* buffer, size_t maxlen);
    virtual size_t write(const uint8_t* buffer, size_t maxlen);
    
    virtual int get();
    virtual void put(uint8_t value);

    virtual size_t available();
};


}

}
