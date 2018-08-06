#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/spi_interface.h"
#include "driver/spi_register.h"

#include <frab/frab.h>

#include <stdio.h>

void spi_test_1();
void spi_test_ili9341();

extern "C" void test_task(void*)
{
    /*
    SpiAttr pAttr;   //Set as Master/Sub mode 0 and speed 2MHz
    pAttr.mode = SpiMode_Master;
    pAttr.subMode = SpiSubMode_0;
    pAttr.speed = SpiSpeed_2MHz;
    // clock divider - SDK seems to do -1 for some reason, so this should
    // be 1 MHz
    //pAttr.speed = (SpiSpeed) (80 - 1);
    pAttr.bitOrder = SpiBitOrder_MSBFirst;
    // TODO: Change SpiNum_HSPI to this->spi at some point, though
    // for ESP8266 prtty sure we are mostly interested in the non-flash SPI
    // (which I *think* actually is the NON HSPI port)
    SPIInit(SpiNum_HSPI, &pAttr);    
    */
    int debug = 0;

    for(;;)
    {
        printf("Debug: %d\n", debug++);
        vTaskDelay(100);
        //spi_test_1();
        spi_test_ili9341();
    }
}