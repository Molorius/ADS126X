
#include "ADS126X.h" // the header
#include "definitions/ADS126X_hardware.h" // board specific commands

/*!< Some initial setup */

ADS126X::ADS126X() {
}

void ADS126X::begin(uint8_t chip_select) {
  cs_used = true;
  cs_pin = chip_select;
  _ads126x_setup_output(cs_pin);
  _ads126x_write_pin_high(cs_pin);
  ADS126X::begin();
}

void ADS126X::begin() {
  /*
  SPI.begin();
  SPI.setDataMode(SPI_MODE1); // see wikipedia
  SPI.setBitOrder(MSBFIRST);
  */
  _ads126x_spi_setup();
  ADS126X::reset(); // reset the board, just in case. This also reads all registers
}

void ADS126X::setStartPin(uint8_t pin) {
  start_used = true;
  start_pin = pin;
  _ads126x_setup_output(start_pin);
  _ads126x_write_pin_low(start_pin);
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
    _ads126x_write_pin_low(start_pin);
    _ads126x_delay(2);
    _ads126x_write_pin_high(start_pin);
  }
  else ADS126X::sendCommand(ADS126X_START1);
}

void ADS126X::stopADC1() {
  ADS126X::sendCommand(ADS126X_STOP1);
}

void ADS126X::startADC2() {
  ADS126X::sendCommand(ADS126X_START2);
}

void ADS126X::stopADC2() {
  ADS126X::sendCommand(ADS126X_STOP2);
}

int32_t ADS126X::readADC1(uint8_t pos_pin,uint8_t neg_pin) {
  if(cs_used) _ads126x_write_pin_low(cs_pin);

  // create buffer to hold transmission
  uint8_t buff[10] = {0}; // plenty of room, all zeros

  union { // create a structure to hold all the data
    struct {
      uint32_t DATA4:8; // bits 0.. 7
      uint32_t DATA3:8; // bits 8.. 15
      uint32_t DATA2:8; // bits 16.. 23
      uint32_t DATA1:8; // bits 24.. 31
    } bit;
    uint32_t reg;
  } ADC_BYTES;
  ADC_BYTES.reg = 0; // clear the ram just in case

  // check if desired pins are different than old pins
  if((REGISTER.INPMUX.bit.MUXN != neg_pin) || (REGISTER.INPMUX.bit.MUXP != pos_pin)) {
    REGISTER.INPMUX.bit.MUXN = neg_pin;
    REGISTER.INPMUX.bit.MUXP = pos_pin;
    ADS126X::writeRegister(ADS126X_INPMUX); // replace on ads126x
  }

  uint8_t i = 0; // current place in outgoing buffer
  buff[i] = ADS126X_RDATA1; // the read adc1 command
  i++;

  if(REGISTER.INTERFACE.bit.STATUS) i++; // place to hold status byte
  i += 4; // place to hold adc data
  if(REGISTER.INTERFACE.bit.CRC>0) i++; // place to hold checksum/crc byte

  _ads126x_spi_rw(buff,i); // write spi, save values on buff

  uint8_t j = 1; // start at byte 1, either status or first adc value

  if(REGISTER.INTERFACE.bit.STATUS) { // if status is being read
    STATUS.reg = buff[j]; // save status byte
    j++; // increment position counter
  }

   // save the data bytes
   ADC_BYTES.bit.DATA1 = buff[j]; j++;
   ADC_BYTES.bit.DATA2 = buff[j]; j++;
   ADC_BYTES.bit.DATA3 = buff[j]; j++;
   ADC_BYTES.bit.DATA4 = buff[j]; j++;

  if(REGISTER.INTERFACE.bit.CRC==ADS126X_CHECKSUM) {
    uint8_t checkbyte = buff[j];
    CHECKSUM = ADS126X::find_checksum(ADC_BYTES.reg,checkbyte);
   }
   else if(REGISTER.INTERFACE.bit.CRC==ADS126X_CHECK_CRC) {
    uint8_t checkbyte = buff[j];
    CHECKSUM = ADS126X::find_crc(ADC_BYTES.reg,checkbyte);
  }

  if(cs_used) _ads126x_write_pin_high(cs_pin);
  return ADC_BYTES.reg;
}

int32_t ADS126X::readADC2(uint8_t pos_pin,uint8_t neg_pin) {
  if(cs_used) _ads126x_write_pin_low(cs_pin);

  // create buffer to hold transmission
  uint8_t buff[10] = {0}; // plenty of room, all zeros

  union { // create a structure to hold all the data
    struct {
      uint32_t DATA3:8; // bits 0.. 7
      uint32_t DATA2:8; // bits 8.. 15
      uint32_t DATA1:8; // bits 16.. 23
      uint32_t :8;      // bits 24.. 31
    } bit;
    uint32_t reg;
  } ADC_BYTES;
  ADC_BYTES.reg = 0; // clear so pad byte is 0

  // check if desired pins are different than old pins
  if((REGISTER.ADC2MUX.bit.MUXN != neg_pin) || (REGISTER.ADC2MUX.bit.MUXP != pos_pin)) {
    REGISTER.ADC2MUX.bit.MUXN = neg_pin;
    REGISTER.ADC2MUX.bit.MUXP = pos_pin;
    ADS126X::writeRegister(ADS126X_ADC2MUX); // replace on ads126x
  }

  uint8_t i = 0; // current place in outgoing buffer
  buff[i] = ADS126X_RDATA2; // the read adc2 command
  i++;

  if(REGISTER.INTERFACE.bit.STATUS) i++; // place to hold status byte
  i += 3; // place to hold adc data
  i++; // place to hold pad byte
  if(REGISTER.INTERFACE.bit.CRC>0) i++; // place to hold checksum/crc byte

  _ads126x_spi_rw(buff,i); // write spi, save values on buff

  uint8_t j = 1; // start at byte 1, either status or first adc value

  if(REGISTER.INTERFACE.bit.STATUS) { // if status is being read
    STATUS.reg = buff[j]; // save status byte
    j++; // increment position counter
  }

   // save the data bytes
   ADC_BYTES.bit.DATA1 = buff[j]; j++;
   ADC_BYTES.bit.DATA2 = buff[j]; j++;
   ADC_BYTES.bit.DATA3 = buff[j]; j++;
   j++; // skip pad byte

  if(REGISTER.INTERFACE.bit.CRC==ADS126X_CHECKSUM) {
    uint8_t checkbyte = buff[j];
    CHECKSUM = ADS126X::find_checksum(ADC_BYTES.reg,checkbyte);
   }
   else if(REGISTER.INTERFACE.bit.CRC==ADS126X_CHECK_CRC) {
    uint8_t checkbyte = buff[j];
    CHECKSUM = ADS126X::find_crc(ADC_BYTES.reg,checkbyte);
  }

  if(cs_used) _ads126x_write_pin_high(cs_pin);
  return ADC_BYTES.reg;
}

void ADS126X::calibrateSysOffsetADC1(uint8_t shorted1,uint8_t shorted2) {
  // connect the pins internally
  REGISTER.INPMUX.bit.MUXN = shorted1;
  REGISTER.INPMUX.bit.MUXP = shorted2;
  ADS126X::writeRegister(ADS126X_INPMUX);
  _ads126x_delay(10); // let signal sort of settle
  ADS126X::sendCommand(ADS126X_SYOCAL1);
  _ads126x_delay(50); // delay to allow time for reads
}

void ADS126X::calibrateGainADC1(uint8_t vcc_pin,uint8_t gnd_pin) {
  REGISTER.INPMUX.bit.MUXN = gnd_pin;
  REGISTER.INPMUX.bit.MUXP = vcc_pin;
  ADS126X::writeRegister(ADS126X_INPMUX);
  _ads126x_delay(100); // delay to allow signal to settle
  ADS126X::sendCommand(ADS126X_SYGCAL1);
  _ads126x_delay(50); // delay to allow time for reads
}

void ADS126X::calibrateSelfOffsetADC1() {
  REGISTER.INPMUX.bit.MUXN = ADS126X_FLOAT;
  REGISTER.INPMUX.bit.MUXP = ADS126X_FLOAT;
  ADS126X::writeRegister(ADS126X_INPMUX);
  _ads126x_delay(10);
  ADS126X::sendCommand(ADS126X_SFOCAL1);
  _ads126x_delay(50);
}

void ADS126X::calibrateSysOffsetADC2(uint8_t shorted1,uint8_t shorted2) {
  REGISTER.ADC2MUX.bit.MUXN = shorted1;
  REGISTER.ADC2MUX.bit.MUXP = shorted2;
  ADS126X::writeRegister(ADS126X_ADC2MUX);
  _ads126x_delay(10);
  ADS126X::sendCommand(ADS126X_SYOCAL2);
  _ads126x_delay(50);
}

void ADS126X::calibrateGainADC2(uint8_t vcc_pin,uint8_t gnd_pin) {
  REGISTER.ADC2MUX.bit.MUXN = gnd_pin;
  REGISTER.ADC2MUX.bit.MUXP = vcc_pin;
  ADS126X::writeRegister(ADS126X_ADC2MUX);
  _ads126x_delay(100);
  ADS126X::sendCommand(ADS126X_SYGCAL2);
  _ads126x_delay(50);
}

void ADS126X::calibrateSelfOffsetADC2() {
  REGISTER.ADC2MUX.bit.MUXN = ADS126X_FLOAT;
  REGISTER.ADC2MUX.bit.MUXP = ADS126X_FLOAT;
  ADS126X::writeRegister(ADS126X_ADC2MUX);
  _ads126x_delay(10);
  ADS126X::sendCommand(ADS126X_SFOCAL2);
  _ads126x_delay(50);
}

/*!< IDAC functionality     */
void ADS126X::setIDAC1Pin(uint8_t pin) {
  #ifndef __AVR__
  REGISTER.IDACMUX.bit.MUX1 = pin;
  #else
  REGISTER.IDACMUX.bit.ADS_MUX1 = pin;
  #endif
  ADS126X::writeRegister(ADS126X_IDACMUX);
}
void ADS126X::setIDAC2Pin(uint8_t pin) {

  #ifndef __AVR__
  REGISTER.IDACMUX.bit.MUX2 = pin;
  #else
  REGISTER.IDACMUX.bit.ADS_MUX2 = pin;
  #endif

  ADS126X::writeRegister(ADS126X_IDACMUX);
}
void ADS126X::setIDAC1Mag(uint8_t magnitude) {
  REGISTER.IDACMAG.bit.MAG1 = magnitude;
  ADS126X::writeRegister(ADS126X_IDACMAG);
}
void ADS126X::setIDAC2Mag(uint8_t magnitude) {
  REGISTER.IDACMAG.bit.MAG2 = magnitude;
  ADS126X::writeRegister(ADS126X_IDACMAG);
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


void ADS126X::setReference(uint8_t negativeReference, uint8_t positiveReference)
{
    REGISTER.REFMUX.bit.RMUXN = negativeReference;
    REGISTER.REFMUX.bit.RMUXP = positiveReference;
    ADS126X::writeRegister(ADS126X_REFMUX);
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
  if(cs_used) _ads126x_write_pin_low(cs_pin);
  uint8_t buff[1] = {command};
  _ads126x_spi_rw(buff,1);
  if(cs_used) _ads126x_write_pin_high(cs_pin);
}

void ADS126X::writeRegisters(uint8_t start_reg,uint8_t num) { // page 87
  if(cs_used) _ads126x_write_pin_low(cs_pin);
  uint8_t buff[50] = {0}; // plenty of room, all zeros

  buff[0] = start_reg | ADS126X_WREG; // first byte is starting register with write command
  buff[1] = num-1; // tell how many registers to write, see datasheet

  // put the desired register values in buffer
  for(uint8_t i=0;i<num;i++) {
    buff[i+2] = REGISTER_ARRAY[i+start_reg];
  }

  // have the microcontroller send the amounts, plus the commands
  _ads126x_spi_rw(buff,num+2);

  if(cs_used) _ads126x_write_pin_high(cs_pin);
}

void ADS126X::readRegisters(uint8_t start_reg,uint8_t num) { // page 86
  if(cs_used) _ads126x_write_pin_low(cs_pin);
  uint8_t buff[50] = {0}; // plenty of room, all zeros

  buff[0] = start_reg | ADS126X_RREG; // first byte is starting register with read command
  buff[1] = num-1; // tell how many registers to read, see datasheet

  _ads126x_spi_rw(buff,num+2); // have the microcontroller read the amounts, plus send the commands

  // save the commands to the register
  for(uint8_t i=0;i<num;i++) {
    REGISTER_ARRAY[i+start_reg] = buff[i+2];
  }

  if(cs_used) _ads126x_write_pin_high(cs_pin);
}

void ADS126X::writeRegister(uint8_t reg) {
  ADS126X::writeRegisters(reg,1);
}

uint8_t ADS126X::readRegister(uint8_t reg) {
  ADS126X::readRegisters(reg,1);
  return REGISTER_ARRAY[reg];
}

uint8_t ADS126X::find_checksum(uint32_t val,uint8_t byt) {
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
  uint64_t num = val; // put val into a 64 bit number
  num <<= 8; // shift by 8
  uint8_t fin = -1; // 8 1's, we need the value to be less than this

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
  return pos;
}
