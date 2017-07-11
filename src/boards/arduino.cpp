/*
Arduino specific functions.
Only compiles if ARDUINO is defined, which is from the compile process. 
*/

#ifdef ARDUINO

#include "../definitions/ADS126X_hardware.h" // header file
#include <Arduino.h>
#include <SPI.h>

// setup microcontroller pin as output
void _ads126x_setup_output(uint8_t pin) {
  pinMode(pin,OUTPUT);
}

// setup microcontroller pin as input
void _ads126x_setup_input(uint8_t pin) {
  pinMode(pin,INPUT);
}

// read microcontroller digital pin
uint8_t _ads126x_read_pin(uint8_t pin) {
  return digitalRead(pin);
}

// write value to microcontroller digital pin
void _ads126x_write_pin_high(uint8_t pin) {
  digitalWrite(pin,HIGH);
}
void _ads126x_write_pin_low(uint8_t pin) {
  digitalWrite(pin,LOW);
}

// setup spi
void _ads126x_spi_setup() {
  SPI.begin();
}

// write buffer to spi, save results over the buffer
void _ads126x_spi_rw(uint8_t buff[],uint8_t len) {
  SPI.beginTransaction( SPISettings(2000000, MSBFIRST, SPI_MODE1) ); // 2 MHz
  
  for(uint8_t i=0;i<len;i++) {
    buff[i] = SPI.transfer(buff[i]);
  }
  
  SPI.endTransaction();
}

// wait for the desired milliseconds
void _ads126x_delay(uint16_t ms) {
  delay(ms);
}

#endif // ifdef ARDUINO
