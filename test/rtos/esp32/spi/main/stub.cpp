#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <frab/esp-idf/spi.h>

using namespace framework_abstraction;

// https://github.com/espressif/esp-idf/blob/master/docs/en/api-reference/peripherals/spi_master.rst

//static layer0::SPI<HSPI_HOST> spi;
static driver::SPI_bus<HSPI_HOST> spi;
static driver::SPI_device<> spi_device;

#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22

#define MAX_TRANSFER_SIZE   1024

void dummy_spi_pre_transfer_callback(spi_transaction_t *t)
{

}

extern "C" void test_task(void*)
{
    // config lifted from
    // https://github.com/espressif/esp-idf/blob/master/examples/peripherals/spi_master/main/spi_master_example_main.c
    spi_bus_config_t buscfg;

    buscfg.miso_io_num=PIN_NUM_MISO;
    buscfg.mosi_io_num=PIN_NUM_MOSI;
    buscfg.sclk_io_num=PIN_NUM_CLK;
    buscfg.quadwp_io_num=-1;
    buscfg.quadhd_io_num=-1;
    buscfg.max_transfer_sz=MAX_TRANSFER_SIZE;

    spi_device_interface_config_t devcfg;
#ifdef CONFIG_LCD_OVERCLOCK
    devcfg.clock_speed_hz=26*1000*1000;           //Clock out at 26 MHz
#else
    devcfg.clock_speed_hz=10*1000*1000;           //Clock out at 10 MHz
#endif
    devcfg.mode=0;                                //SPI mode 0
    devcfg.spics_io_num=PIN_NUM_CS;               //CS pin
    devcfg.queue_size=7;                          //We want to be able to queue 7 transactions at a time
    devcfg.pre_cb=dummy_spi_pre_transfer_callback;  //Specify pre-transfer callback to handle D/C line

    spi.initialize(buscfg, 1);
    spi.add(spi_device, devcfg);

    for(;;)
    {
    }
}