#include <frab/gpio.h>


using namespace framework_abstraction;

layer1::digital_out<0> led;

void toggle()
{
    static bool value = false;

    led.write(value = !value);
}