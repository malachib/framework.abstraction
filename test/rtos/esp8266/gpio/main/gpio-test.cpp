#include <estd/port/identify_platform.h>

// This is necessary as it picks up the sdkconfig.h during ESP8266 compile
#include "freertos/FreeRTOS.h"

#include <frab/gpio.h>


using namespace framework_abstraction;

// NOTE: using gpio_traits directly not really how C++ libs usually
// do things.
CONSTEXPR pin_t led_gpio = gpio_traits::to_pin(CONFIG_TEST_GPIO);

// TODO: find a way to avoid this typecast (required during esp32 compile)
layer0::digital_out<led_gpio> led;

void toggle()
{
    static bool value = false;

    led.write(value = !value);
}