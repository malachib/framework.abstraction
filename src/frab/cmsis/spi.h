#pragma once

namespace framework_abstraction {

namespace driver {

// layer1-ish , to conform to frab signature
// async behavior still not locked down
template <ARM_DRIVER_SPI& native>
class SPI
{
protected:
    static void signal_event(uint32_t event)
    {
        if(event & ARM_SPI_EVENT_TRANSFER_COMPLETE)
        {

        }
        if(event & ARM_SPI_EVENT_DATA_LOST)
        {

        }
        if(event & ARM_SPI_EVENT_MODE_FAULT)
        {

        }
    }

public:
    SPI()
    {
        native.Initialize(signal_event);
    }

    ~SPI()
    {
        native.Uninitialize();
    }

    void read(void* data, uint32_t num)
    {
        native.Receive(data, num);
    }

    void get_status()
    {
        native.GetStatus();
    }

    uint32_t get_data_count()
    {
        return native.GetDataCount();
    }
};

/*
class SPI
{
protected:
    ARM_DRIVER_SPI& native;

public:
    SPI(ARM_DRIVER_SPI& native) : native(native) {}


}; */

}

}