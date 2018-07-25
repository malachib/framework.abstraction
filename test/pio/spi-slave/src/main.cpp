#include <Arduino.h>

// brings in fully untested arduino code and breaks
//#include <frab/spi.h>
#include <frab/avr/spi-slave.hpp>
#include <estd/string.h>

void setup()
{
    spi_slave_setup();
}

void loop()
{

}