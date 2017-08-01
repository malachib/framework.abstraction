#include <mbed.h>

#include <frab/gpio.h>
#include <frab/spi.h>

using namespace framework_abstraction;

layer1::digital_out<NC> do1;
layer1::digital_in<NC> di1;
layer2::digital_io dio2(NC);

hal::SPI spi(NC, NC, NC);
//mbed::SPI spi(NC, NC, NC);

int main()
{
	do1.write(1);
	do1.write(0);
	di1.read();
	di1.read();
	di1.mode(gpio::pull_up);
	di1.mode(gpio::pull_up);

	return 0;
}
