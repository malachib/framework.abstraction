#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <frab/esp-idf/spi.h>

using namespace framework_abstraction;

// https://github.com/espressif/esp-idf/blob/master/docs/en/api-reference/peripherals/spi_master.rst

//static layer0::SPI<HSPI_HOST> spi;
static driver::SPI<HSPI_HOST> spi;

extern "C" void test_task(void*)
{
    for(;;)
    {

    }
}