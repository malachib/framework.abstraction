#include <iostream>

#include <frab/systime.h>

int main()
{
    auto value = frab::millis();
    std::cout << "Hello, World: " << value << std::endl;
    value = frab::millis();
    std::cout << "Hello, World: " << value << std::endl;
    return 0;
}