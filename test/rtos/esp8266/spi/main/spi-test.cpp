#include <frab/spi.h>
#include <frab/gpio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <estd/chrono.h>

#include "time.h"
#include <sys/time.h> 

using namespace framework_abstraction;
using namespace estd;

hal::SPI spi;
layer1::SPI<SpiNum_HSPI> spi2;

void spi_test_1()
{
    char buffer[] = "Hello 2 u";

    spi2.begin();
    spi2.set_order_msb(true);
    spi2.write(buffer, sizeof(buffer) - 1);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

static layer1::digital_out<4> pin_dc;

inline void delay_us(uint16_t delay)
{
    // apparently this is gone
    // ala https://esp32.com/viewtopic.php?f=2&t=859&p=3626#p3626
    //system_get_rtc_time();
    struct timeval tv_start = { .tv_sec = 0, .tv_usec = 0 };   /* btw settimeofday() is helpfull here too*/
    struct timeval tv = { .tv_sec = 0, .tv_usec = 0 };   /* btw settimeofday() is helpfull here too*/

    gettimeofday(&tv_start, NULL);

    do 
    {
        gettimeofday(&tv, NULL);
    }
    while((tv.tv_usec - tv_start.tv_usec) < delay);
}


inline void ili9341_cmd8(uint8_t cmd)
{
    pin_dc.write(false); // low for command

    // pretty big delay, example code actually only used 5us
    // for testing purposes should be OK though
    //vTaskDelay(1);
    delay_us(5);

    // note our SPI approach waits at the beginning of the send, not the end
    spi2.write8(cmd);
}


inline void ili9341_data8(uint8_t data)
{
    // just to make sure we send the darned command before doing a dc=0 again
    spi2.busy_wait();

    pin_dc.write(true); // high for data

    delay_us(1);

    // note our SPI approach waits at the beginning of the send, not the end
    spi2.write8(data);

}


void ili9341_setaddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)//set coordinate for print or other function
{
	ili9341_cmd8(0x2A);
	ili9341_data8(x1>>8);
	ili9341_data8(x1);
	ili9341_data8(x2>>8);
	ili9341_data8(x2);

	ili9341_cmd8(0x2B);
	ili9341_data8(y1>>8);
	ili9341_data8(y1);
	ili9341_data8(y2);
	ili9341_data8(y2);

	ili9341_cmd8(0x2C);//meory write
}

void ili9341_pushcolour(uint16_t colour)
{
	ili9341_data8(colour>>8);
	ili9341_data8(colour);
}


#define LCD_W 320
#define LCD_H 240

void ili9341_drawpixel(uint16_t x3,uint16_t y3,uint16_t colour1) //pixels will always be counted from right side.x is representing LCD width which will always be less tha 240.Y is representing LCD height which will always be less than 320
{
    /*
	if ((x3 < 0) ||(x3 >= LCD_W) || (y3 < 0) || (y3 >= LCD_H))
	{
		return;
	} */

	ili9341_setaddress(x3,y3,x3+1,y3+1);
	ili9341_pushcolour(colour1);
}

// test code lifted from 
// https://gist.github.com/postmodern/ed6e670999f456ad9f13
void spi_test_ili9341()
{
    spi2.begin();

    //disable MOSI, MISO, ADDR, COMMAND, DUMMY in case previously set.
	CLEAR_PERI_REG_MASK(SPI_USER(SpiNum_HSPI), SPI_USR_MOSI|SPI_USR_MISO|SPI_USR_COMMAND|SPI_USR_ADDR|SPI_USR_DUMMY);
    SET_PERI_REG_MASK(SPI_USER(SpiNum_HSPI), SPI_USR_MOSI); //enable MOSI function in SPI module

    for(int i = 0; i <100; i++)
        ili9341_drawpixel(i, i, 0xFFFF);
}