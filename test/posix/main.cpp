#include <iostream>

#include <frab/systime.h>
#include <frab/gpio.h>

namespace frab = framework_abstraction;

int main()
{
    auto value = frab::millis();
    std::cout << "Hello, World: " << value << std::endl;
    value = frab::millis();
    std::cout << "Hello, World: " << value << std::endl;
    return 0;
}