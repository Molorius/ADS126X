
// All functions that are microcontroller/framework specific. 

#ifndef ADS126X_HARDWARE_H
#define ADS126X_HARDWARE_H

#include <stdint.h>

// setup microcontroller pins as input/output
void _ads126x_setup_output(uint8_t pin);
void _ads126x_setup_input(uint8_t pin);

// read microcontroller digital pin
uint8_t _ads126x_read_pin(uint8_t pin);

// write microcontroller pin high and low
void _ads126x_write_pin_high(uint8_t pin);
void _ads126x_write_pin_low(uint8_t pin);

// setup spi
void _ads126x_spi_setup();

// write buffer to spi, save results over the buffer
void _ads126x_spi_rw(uint8_t buff[],uint8_t len);

// wait for the desired milliseconds
void _ads126x_delay(uint16_t ms);

#endif // define ADS126X_HARDWARE_H
