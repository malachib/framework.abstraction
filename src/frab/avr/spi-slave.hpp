#include "spi-slave.h"

#include <avr/io.h>
#include <avr/interrupt.h>

// code adapted from:
//  https://forum.arduino.cc/index.php?topic=525449.0
//  https://gist.github.com/chrismeyersfsu/3317769
//  https://sites.google.com/site/qeewiki/books/avr-guide/spi
// with helpful guidance from:
//  http://maxembedded.com/2013/11/the-spi-of-the-avr/#Advantages
//  https://www.avrfreaks.net/forum/spi-atmega32u4-and-eeprom

estd::layer1::queue<uint8_t, FRAB_SPISLAVE_BUFFER_SIZE> spi_slave_in;
#ifdef FEATURE_FRAB_SPISLAVE_DUPLEX
estd::layer1::queue<uint8_t, FRAB_SPISLAVE_BUFFER_SIZE> spi_slave_out;
#endif

// SPI interrupt routine - only job in life is to populate
// incoming queue
ISR (SPI_STC_vect)
{
    uint8_t c = SPDR; // read byte from SPI Data Register
    spi_slave_in.push(c);

    // TODO: Manage an 'overflow' flag

#ifdef FEATURE_FRAB_SPISLAVE_DUPLEX
    SPDR = spi_slave_out.front();
    spi_slave_out.pop();
#endif
}

static void spi_slave_setup()
{
    using namespace framework_abstraction;

    internal::port<spi_pins::port> port;

    port.as_input(spi_pins::sck, spi_pins::mosi, spi_pins::cs);
    port.as_output(spi_pins::miso);

    // This is AVR ATMega168/328 code - not sure if it will work properly
    // (especially pin select) for 32u4
    //DDRB &= ~((1<<2)|(1<<3)|(1<<5));   // SCK, MOSI and SS as inputs

    // http://maxembedded.com/2013/11/the-spi-of-the-avr/#Advantages indicates
    // 1 << 6 should be output, so we do have differences between the AVRs
    //DDRB |= (1<<4);                    // MISO as output

    // turn on SPI in slave mode
    SPCR |= _BV(SPE);

    // Enable SPI Interrupt
    SPCR |= (1<<SPIE);

    // Enable SPI
    SPCR |= (1<<SPE);

    // enable global interrupts
    sei();
}