#include <iostream>

#include <frab/systime.h>

int main()
{
    auto value = frab::millis();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}