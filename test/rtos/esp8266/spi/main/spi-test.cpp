#include <frab/spi.h>

using namespace framework_abstraction;

framework_abstraction::hal::SPI spi;

void spi_test_1()
{
    layer1::SPI<SpiNum_HSPI> spi2;
    char buffer[] = "Hello 2 u";

    spi2.write(buffer, sizeof(buffer) - 1);
}