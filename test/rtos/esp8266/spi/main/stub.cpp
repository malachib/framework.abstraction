#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <frab/frab.h>

void spi_test_1();

extern "C" void test_task(void*)
{
    for(;;)
    {
        vTaskDelay(100);
        spi_test_1();
    }
}