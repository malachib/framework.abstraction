#include <frab/spi.h>
#include <frab/gpio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

using namespace framework_abstraction;

framework_abstraction::hal::SPI spi;

void spi_test_1()
{
    layer1::SPI<SpiNum_HSPI> spi2;
    char buffer[] = "Hello 2 u";

    spi2.begin();
    spi2.set_order_msb(true);
    spi2.write(buffer, sizeof(buffer) - 1);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void spi_test_ili9341()
{
    layer1::SPI<SpiNum_HSPI> spi2;

    spi2.begin();
}