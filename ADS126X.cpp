#include "ADS126X.h"
#include <Arduino.h> // for digital read/write
#include <SPI.h> // for all spi commands

/*!< Some initial setup */

ADS126X::ADS126X() {
}

void ADS126X::begin(uint8_t chip_select) {
  cs_used = true;
  cs_pin = chip_select;
  pinMode(cs_pin,OUTPUT);
  digitalWrite(cs_pin,1);
  ADS126X::begin();
}

void ADS126X::begin() {
  SPI.begin();
  SPI.setDataMode(SPI_MODE1); // see wikipedia
  ADS126X::reset(); // reset the board, just in case. This also reads all registers
}

void ADS126X::setStartPin(uint8_t pin) {
  start_used = true;
  start_pin = pin;
  pinMode(start_pin,OUTPUT);
  digitalWrite(start_pin,0);
}

/*!< Regular ADC Commands    */

void ADS126X::noOperation() {
  ADS126X::sendCommand(ADS126X_NOP);
}

void ADS126X::reset() {
  sendCommand(ADS126X_RESET); // send reset command
  ADS126X::readRegisters(0,ADS126X_REG_NUM); // read all the registers
}

void ADS126X::startADC1() {
  if(start_used) {
    digitalWrite(start_pin,0); // for when running in pulse-conversion
    delay(2);
    digitalWrite(start_pin,1);
  }
  else ADS126X::sendCommand(ADS126X_START1);
}

void ADS126X::stopADC1() {
  if(start_used) {
    digitalWrite(start_pin,0);
  }
  else ADS126X::sendCommand(ADS126X_STOP1);
}

void ADS126X::startADC2() {
  ADS126X::sendCommand(ADS126X_START2);
}

void ADS126X::stopADC2() {
  ADS126X::sendCommand(ADS126X_STOP2);
}

int32_t ADS126X::readADC1(uint8_t pos_pin,uint8_t neg_pin) {
  // I have this by command only
  if(cs_used) digitalWrite(cs_pin,0);

  union { // create a structure to hold all the data
    struct {
      uint32_t DATA4:8; // bits 0.. 7
      uint32_t DATA3:8; // bits 8.. 15
      uint32_t DATA2:8; // bits 16.. 23
      uint32_t DATA1:8; // bits 24.. 31
    } bit;
    uint32_t reg;
  } ADC_BYTES;
  
  // check if desired pins are different than old pins
  if((REGISTER.INPMUX.bit.MUXN != neg_pin) || (REGISTER.INPMUX.bit.MUXP != pos_pin)) {
    REGISTER.INPMUX.bit.MUXN = neg_pin;
    REGISTER.INPMUX.bit.MUXP = pos_pin;
    ADS126X::writeRegister(ADS126X_INPMUX); // replace on ads126x    
  }
  
  SPI.transfer(ADS126X_RDATA1); // send the read data command

  if(REGISTER.INTERFACE.bit.STATUS) { // if the status byte is enabled
    STATUS.reg = SPI.transfer(ADS126X_DUMMY); // save the first bit
  }

  // collect the data bits
  ADC_BYTES.bit.DATA1 = SPI.transfer(ADS126X_DUMMY);
  ADC_BYTES.bit.DATA2 = SPI.transfer(ADS126X_DUMMY);
  ADC_BYTES.bit.DATA3 = SPI.transfer(ADS126X_DUMMY);
  ADC_BYTES.bit.DATA4 = SPI.transfer(ADS126X_DUMMY);

  if(REGISTER.INTERFACE.bit.CRC==ADS126X_CHECKSUM) { // if checksum or crc is enabled, perform check
    uint8_t checkbyte = SPI.transfer(ADS126X_DUMMY);
    CHECKSUM = ADS126X::find_checksum(ADC_BYTES.reg,checkbyte);
  }
  else if(REGISTER.INTERFACE.bit.CRC==ADS126X_CHECK_CRC) {
    uint8_t checkbyte = SPI.transfer(ADS126X_DUMMY);
    CHECKSUM = ADS126X::find_crc(ADC_BYTES.reg,checkbyte);
  }

  if(cs_used) digitalWrite(cs_pin,1);
  return ADC_BYTES.reg;
}

int32_t ADS126X::readADC2(uint8_t pos_pin,uint8_t neg_pin) {
  if(cs_used) digitalWrite(cs_pin,0);

  union { // create a structure to hold all the data
    struct {
      uint32_t DATA3:8; // bits 0.. 7
      uint32_t DATA2:8; // bits 8.. 15
      uint32_t DATA1:8; // bits 16.. 23
      uint32_t :8;      // bits 24.. 31
    } bit;
    uint32_t reg;
  } ADC_BYTES;
  
  // check if desired pins are different than old pins
  if((REGISTER.ADC2MUX.bit.MUXN != neg_pin) || (REGISTER.ADC2MUX.bit.MUXP != pos_pin)) {
    REGISTER.ADC2MUX.bit.MUXN = neg_pin;
    REGISTER.ADC2MUX.bit.MUXP = pos_pin;
    ADS126X::writeRegister(ADS126X_ADC2MUX); // replace on ads126x    
  }
  
  SPI.transfer(ADS126X_RDATA2); // send the read data command

  if(REGISTER.INTERFACE.bit.STATUS) { // if the status byte is enabled
    STATUS.reg = SPI.transfer(ADS126X_DUMMY); // save the first bit
  }

  // collect the data bits
  ADC_BYTES.bit.DATA1 = SPI.transfer(ADS126X_DUMMY);
  ADC_BYTES.bit.DATA2 = SPI.transfer(ADS126X_DUMMY);
  ADC_BYTES.bit.DATA3 = SPI.transfer(ADS126X_DUMMY);
  SPI.transfer(ADS126X_DUMMY); // zero pad byte

  if(REGISTER.INTERFACE.bit.CRC==ADS126X_CHECKSUM) { // if checksum or crc is enabled, perform check
    uint8_t checkbyte = SPI.transfer(ADS126X_DUMMY);
    CHECKSUM = ADS126X::find_checksum(ADC_BYTES.reg,checkbyte);
  }
  else if(REGISTER.INTERFACE.bit.CRC==ADS126X_CHECK_CRC) {
    uint8_t checkbyte = SPI.transfer(ADS126X_DUMMY);
    CHECKSUM = ADS126X::find_crc(ADC_BYTES.reg,checkbyte);
  }

  if(cs_used) digitalWrite(cs_pin,1);
  return ADC_BYTES.reg;
}

void ADS126X::calibrateSysOffsetADC1(uint8_t shorted1,uint8_t shorted2) {
  // connect the pins internally
  REGISTER.INPMUX.bit.MUXN = shorted1;
  REGISTER.INPMUX.bit.MUXP = shorted2;
  ADS126X::writeRegister(ADS126X_INPMUX);
  delay(10); // let signal sort of settle
  ADS126X::sendCommand(ADS126X_SYOCAL1);
  delay(50); // delay to allow time for reads
}

void ADS126X::calibrateGainADC1(uint8_t vcc_pin,uint8_t gnd_pin) {
  REGISTER.INPMUX.bit.MUXN = gnd_pin;
  REGISTER.INPMUX.bit.MUXP = vcc_pin;
  ADS126X::writeRegister(ADS126X_INPMUX);
  delay(100); // delay to allow signal to settle
  ADS126X::sendCommand(ADS126X_SYGCAL1);
  delay(50); // delay to allow time for reads
}

void ADS126X::calibrateSelfOffsetADC1() {
  REGISTER.INPMUX.bit.MUXN = ADS126X_FLOAT;
  REGISTER.INPMUX.bit.MUXP = ADS126X_FLOAT;
  ADS126X::writeRegister(ADS126X_INPMUX);
  delay(10);
  ADS126X::sendCommand(ADS126X_SFOCAL1);
  delay(50);
}

void ADS126X::calibrateSysOffsetADC2(uint8_t shorted1,uint8_t shorted2) {
  REGISTER.ADC2MUX.bit.MUXN = shorted1;
  REGISTER.ADC2MUX.bit.MUXP = shorted2;
  ADS126X::writeRegister(ADS126X_ADC2MUX);
  delay(10);
  ADS126X::sendCommand(ADS126X_SYOCAL2);
  delay(50);
}

void ADS126X::calibrateGainADC2(uint8_t vcc_pin,uint8_t gnd_pin) {
  REGISTER.ADC2MUX.bit.MUXN = gnd_pin;
  REGISTER.ADC2MUX.bit.MUXP = vcc_pin;
  ADS126X::writeRegister(ADS126X_ADC2MUX);
  delay(100);
  ADS126X::sendCommand(ADS126X_SYGCAL2);
  delay(50);
}

void ADS126X::calibrateSelfOffsetADC2() {
  REGISTER.ADC2MUX.bit.MUXN = ADS126X_FLOAT;
  REGISTER.ADC2MUX.bit.MUXP = ADS126X_FLOAT;
  ADS126X::writeRegister(ADS126X_ADC2MUX);
  delay(10);
  ADS126X::sendCommand(ADS126X_SFOCAL2);
}

/*!< POWER register	        */

bool ADS126X::checkResetBit() {
	ADS126X::readRegister(ADS126X_POWER); // read the POWER register
	return REGISTER.POWER.bit.RESET;
}

void ADS126X::clearResetBit() {
	REGISTER.POWER.bit.RESET = 0;
	ADS126X::writeRegister(ADS126X_POWER);
}

void ADS126X::enableLevelShift() {
	REGISTER.POWER.bit.VBIAS = 1;
	ADS126X::writeRegister(ADS126X_POWER);
}

void ADS126X::disableLevelShift() {
	REGISTER.POWER.bit.VBIAS = 0;
	ADS126X::writeRegister(ADS126X_POWER);
}

void ADS126X::enableInternalReference() {
	REGISTER.POWER.bit.INTREF = 1;
	ADS126X::writeRegister(ADS126X_POWER);
}

/*!< INTERFACE register     */

void ADS126X::disableCheck() {
  CHECKSUM = 0;
  REGISTER.INTERFACE.bit.CRC = ADS126X_DISABLE;
  ADS126X::writeRegister(ADS126X_INTERFACE);
}

void ADS126X::setChecksumMode() {
  REGISTER.INTERFACE.bit.CRC = ADS126X_CHECKSUM;
  ADS126X::writeRegister(ADS126X_INTERFACE);
}

void ADS126X::setCRCMode() {
  REGISTER.INTERFACE.bit.CRC = ADS126X_CHECK_CRC;
  ADS126X::writeRegister(ADS126X_INTERFACE);
}

bool ADS126X::lastChecksum() {
  return CHECKSUM;
}


/*!< Status Functions 		*/

void ADS126X::enableStatus() {
  REGISTER.INTERFACE.bit.STATUS = ADS126X_ENABLE;
  ADS126X::writeRegister(ADS126X_INTERFACE);
}

void ADS126X::disableStatus() {
  STATUS.reg = 0;
  REGISTER.INTERFACE.bit.STATUS = ADS126X_DISABLE;
  ADS126X::writeRegister(ADS126X_INTERFACE);
}

uint8_t ADS126X::lastStatus() {
  return STATUS.reg;
}

bool ADS126X::lastADC2Status() {
	return STATUS.bit.ADC2;
}

bool ADS126X::lastADC1Status() {
	return STATUS.bit.ADC1;
}

bool ADS126X::lastClockSource() {
	return STATUS.bit.EXTCLK;
}

bool ADS126X::lastADC1LowReferenceAlarm() {
	return STATUS.bit.REF_ALM;
}

bool ADS126X::lastADC1PGAOutputLowAlarm() {
	return STATUS.bit.PGAL_ALM;
}

bool ADS126X::lastADC1PGAOutputHighAlarm() {
	return STATUS.bit.PGAH_ALM;
}

bool ADS126X::lastADC1PGADifferentialOutputAlarm() {
	return STATUS.bit.PGAD_ALM;
}

bool ADS126X::lastReset() {
	return STATUS.bit.RESET;
}

/*!< MODE0 register       */

void ADS126X::setContinuousMode() {
  REGISTER.MODE0.bit.RUNMODE = ADS126X_CONV_CONT;
  ADS126X::writeRegister(ADS126X_MODE0);
}

void ADS126X::setPulseMode() {
  REGISTER.MODE0.bit.RUNMODE = ADS126X_CONV_PULSE;
  ADS126X::writeRegister(ADS126X_MODE0);
}

void ADS126X::setChopMode(uint8_t mode) {
  REGISTER.MODE0.bit.CHOP = mode;
  ADS126X::writeRegister(ADS126X_MODE0);
}

void ADS126X::setDelay(uint8_t del) {
  REGISTER.MODE0.bit.DELAY = del;
  ADS126X::writeRegister(ADS126X_MODE0);
}

/*!< MODE1 register       */

void ADS126X::setFilter(uint8_t filter) {
  REGISTER.MODE1.bit.FILTER = filter;
  ADS126X::writeRegister(ADS126X_MODE1);
}

void ADS126X::setBiasADC(uint8_t adc_choice) {
  REGISTER.MODE1.bit.SBADC = adc_choice;
  ADS126X::writeRegister(ADS126X_MODE1);
}

void ADS126X::setBiasPolarity(uint8_t polarity) {
  REGISTER.MODE1.bit.SBPOL = polarity;
  ADS126X::writeRegister(ADS126X_MODE1);
}

void ADS126X::setBiasMagnitude(uint8_t mag) {
  REGISTER.MODE1.bit.SBMAG = mag;
  ADS126X::writeRegister(ADS126X_MODE1);
}

/*!< MODE2 register       */

void ADS126X::enablePGA() {
  REGISTER.MODE2.bit.BYPASS = ADS126X_PGA_ENABLE;
  ADS126X::writeRegister(ADS126X_MODE2);
}

void ADS126X::bypassPGA() {
  REGISTER.MODE2.bit.BYPASS = ADS126X_PGA_BYPASS;
  ADS126X::writeRegister(ADS126X_MODE2);
}

void ADS126X::setGain(uint8_t gain) {
  REGISTER.MODE2.bit.GAIN = gain;
  ADS126X::writeRegister(ADS126X_MODE2);
}

void ADS126X::setRate(uint8_t rate) {
  REGISTER.MODE2.bit.DR = rate;
  ADS126X::writeRegister(ADS126X_MODE2);
}

/*!< GPIO commands        */

void ADS126X::gpioConnect(uint8_t pin) {
  REGISTER.GPIOCON.reg |= (1<<pin); // set value to 1
  ADS126X::writeRegister(ADS126X_GPIOCON);
}

void ADS126X::gpioDisconnect(uint8_t pin) {
  REGISTER.GPIOCON.reg |= (1<<pin); // set value to 1
  REGISTER.GPIOCON.reg ^= (1<<pin); // force pin to 0
  ADS126X::writeRegister(ADS126X_GPIOCON);
}

void ADS126X::gpioDirection(uint8_t pin,uint8_t direction) {
  REGISTER.GPIODIR.reg |= (1<<pin); // set value to 1
  if(!direction) { // if it's 0
    REGISTER.GPIODIR.reg ^= (1<<pin); // set value to 0
  }
  ADS126X::writeRegister(ADS126X_GPIODIR);
}

void ADS126X::gpioWrite(uint8_t pin,uint8_t val) {
  REGISTER.GPIODAT.reg |= (1<<pin); // set value to 1
  if(!val) { // if it's 0
    REGISTER.GPIODAT.reg ^= (1<<pin); // set value to 0
  }
  ADS126X::writeRegister(ADS126X_GPIODAT);
}

bool ADS126X::gpioRead(uint8_t pin) {
  ADS126X::readRegister(ADS126X_GPIODAT); // read register
  uint8_t mask = 1<<pin;
  return (REGISTER.GPIODAT.reg & mask);
}

/*!< Backend commands     */

void ADS126X::sendCommand(uint8_t command) {
  if(cs_used) digitalWrite(cs_pin,0);
  SPI.transfer(command);
  if(cs_used) digitalWrite(cs_pin,1);
}

void ADS126X::writeRegisters(uint8_t start_reg,uint8_t num) { // page 87
  if(cs_used) digitalWrite(cs_pin,0);
  uint8_t command = start_reg | ADS126X_WREG; // couple with the write command
  SPI.transfer(command); // tell the device the starting register
  SPI.transfer(num-1); // tell how many registers we want to write
  
  for(uint8_t i=0;i<num;i++) { // for each register we want to write
    uint8_t current_reg = i+start_reg; // find the current register
    SPI.transfer(REGISTER_ARRAY[current_reg]); // write the register
  }
  if(cs_used) digitalWrite(cs_pin,1);
}

void ADS126X::readRegisters(uint8_t start_reg,uint8_t num) { // page 86
  if(cs_used) digitalWrite(cs_pin,0);
  
  uint8_t command = start_reg | ADS126X_RREG; // couple with the read command
  SPI.transfer(command); // tell the device the starting register
  SPI.transfer(num-1); // tell how many registers to read

  for(uint8_t i=0;i<num;i++) { // for each register we want to read
    uint8_t current_reg = i+start_reg; // find the current desired register
    REGISTER_ARRAY[current_reg] = SPI.transfer(ADS126X_DUMMY); // read and save the register
  }
  if(cs_used) digitalWrite(cs_pin,1);
}

void ADS126X::writeRegister(uint8_t reg) {
  ADS126X::writeRegisters(reg,1);
}

uint8_t ADS126X::readRegister(uint8_t reg) {
  ADS126X::readRegisters(reg,1);
  return REGISTER_ARRAY[reg];
}

uint8_t ADS126X::find_checksum(uint32_t val,uint8_t byt) {
  Serial.println("check"); delay(100);
  uint8_t sum = 0;
  uint8_t mask = -1; // 8 bit mask of all 1s
  while(val) {
    sum += val & mask; // only add the lower values
    val >>= 8; // shift everything down
  }
  sum += ADS126X_CHECK_BYTE;
  return sum ^ byt; // if equal, this will be 0
}

uint8_t ADS126X::find_crc(uint32_t val,uint8_t byt) {
  Serial.println("crc"); delay(100);
  uint64_t num = val; // put val into a 64 bit number
  num <<= 8; // shift by 8
  uint8_t fin = -1; // 8 1's, we need the value to be less than this

  long n = 0;
  while( (num & fin) ^ num ) { // while num is greater than 8 bits
    uint8_t msb_pos = ADS126X::msb_pos(num); // find the position of the greatest bit
    uint64_t divisor = ADS126X_CRC_BYTE; // set the divisor to 64 bit
    divisor <<= (msb_pos-9); // shift divisor to match greatest bit
    num ^= divisor; // XOR it
  }
  return num ^ byt; // if equal, this will be 0
}

uint8_t ADS126X::msb_pos(uint64_t val) { 
  // returns position such that the zeroth bit will give 1. 
  // This is so an input of 0 will return 0. 
  uint8_t pos = 1;
  while(val >>= 1) pos++;
  //pos *= (val && 1); // if val is 0, pos will be 0
  return pos;
}

