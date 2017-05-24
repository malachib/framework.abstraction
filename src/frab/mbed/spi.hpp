// designed to ONLY be included from spi_shim.  Don't include otherwise!
template <>
inline void SPI<mbed::SPI>::format(uint8_t bits, spi_modes mode)
{
    spi.format(bits, mode);
}

template <>
inline void SPI<mbed::SPI>::frequency(int32_t hz)
{
    spi.frequency(hz);
}


template <>
inline void SPI<mbed::SPI>::begin() {}

template <>
inline void SPI<mbed::SPI>::end() {}


template <>
template <class T>
inline void SPI<mbed::SPI>::transfer(const T* tx_buffer, size_t tx_length, T* rx_buffer, size_t rx_length,
                                     async_context_t async_context)
{
    spi.transfer(tx_buffer, tx_length, rx_buffer, rx_length, async_context.callback, async_context.event);
}


namespace hal
{

typedef frab::SPI<mbed::SPI> SPI;

}