#include <estd/port/identify_platform.h>

#include <frab/gpio.h>


using namespace framework_abstraction;

// TODO: find a way to avoid this typecast (required during esp32 compile)
layer0::digital_out<(gpio_num_t)CONFIG_TEST_GPIO> led;

void toggle()
{
    static bool value = false;

    led.write(value = !value);
}