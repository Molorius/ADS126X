/*
Blake Felt
https://github.com/Molorius/ADS126X-Arduino

This is a class to use the TI ADS1262 and ADS1263 analog converters with an Arduino.
*/

#ifndef ADS126X_H
#define ADS126X_H

#include <stdint.h>
#include "definitions/ADS126X_definitions.h"

class ADS126X {
  public:
  	// Initialization
    ADS126X(void);
    void begin(uint8_t chip_select);
    void begin(void);
    void setStartPin(uint8_t pin); // designate a pin connected to START

    // All ADC Commands. Page 85
    //General Commands
    void noOperation(void);
    void reset(void);
    void startADC1(void);
    void stopADC1(void);
    void startADC2(void);
    void stopADC2(void);
    // Analog Read Functions
    int32_t readADC1(uint8_t pos_pin,uint8_t neg_pin);
    int32_t readADC2(uint8_t pos_pin,uint8_t neg_pin);
    // Calibration Functions
    void calibrateSysOffsetADC1(uint8_t shorted1,uint8_t shorted2);
    void calibrateGainADC1(uint8_t vcc_pin,uint8_t gnd_pin);
    void calibrateSelfOffsetADC1(void);
    void calibrateSysOffsetADC2(uint8_t shorted1,uint8_t shorted2);
    void calibrateGainADC2(uint8_t vcc_pin,uint8_t gnd_pin);
    void calibrateSelfOffsetADC2(void);
    // IDAC Functions
    void setIDAC1Pin(uint8_t pin);
    void setIDAC2Pin(uint8_t pin);
    void setIDAC1Mag(uint8_t magnitude);
    void setIDAC2Mag(uint8_t magnitude);


    // POWER functions
    bool checkResetBit(void);
    void clearResetBit(void);
    void enableLevelShift(void);
    void disableLevelShift(void);
    void enableInternalReference(void);
    void disableInternalReference(void);

    // INTERFACE/checksum functions
    void disableCheck(void);
    void setChecksumMode(void);
    void setCRCMode(void);
    bool lastChecksum(void);

    // Status functions
    void enableStatus(void);
    void disableStatus(void);
    uint8_t lastStatus(void); // returns entire status byte
    bool lastADC2Status(void);
    bool lastADC1Status(void);
    bool lastClockSource(void);
    bool lastADC1LowReferenceAlarm(void);
    bool lastADC1PGAOutputLowAlarm(void);
    bool lastADC1PGAOutputHighAlarm(void);
    bool lastADC1PGADifferentialOutputAlarm(void);
    bool lastReset(void);

    // MODE0 functions
    void setContinuousMode(void);
    void setPulseMode(void);
    void setChopMode(uint8_t mode);
    void setDelay(uint8_t del);

    // MODE1 functions
    void setFilter(uint8_t filter);
    void setBiasADC(uint8_t adc_choice);
    void setBiasPolarity(uint8_t polarity);
    void setBiasMagnitude(uint8_t mag);

    // MODE2 functions
    void enablePGA(void);
    void bypassPGA(void);
    void setGain(uint8_t gain);
    void setRate(uint8_t rate);

    void setReference(uint8_t negativeReference, uint8_t positiveReference);

    // GPIO functions
    void gpioConnect(uint8_t pin);
    void gpioDisconnect(uint8_t pin);
    void gpioDirection(uint8_t pin,uint8_t direction);
    void gpioWrite(uint8_t pin,uint8_t val);
    bool gpioRead(uint8_t pin);

  //private:
    // This will hold all values of the register. All commands are done
    // through this.
    ADS126X_REGISTER_Type REGISTER;
    // An array to call each register by the offset.
    // It has the same memory location as REGISTER, so the changes
    // will reflect on both.
    __IO unsigned char *REGISTER_ARRAY = (uint8_t *)&REGISTER.ID.reg;

    bool cs_used = false;
    uint8_t cs_pin; // chip select pin
    bool start_used = false;
    uint8_t start_pin; // start pin

    ADS126X_STATUS_Type STATUS; // save last status and checksum values
    uint8_t CHECKSUM;

    void sendCommand(uint8_t command); // sends a single command
    void readRegisters(uint8_t start_reg,uint8_t num);
    void writeRegisters(uint8_t start_reg,uint8_t num);
    uint8_t readRegister(uint8_t reg);
    void writeRegister(uint8_t reg);

    // checksum math, page 72
    uint8_t find_checksum(uint32_t val,uint8_t byt);
    uint8_t find_crc(uint32_t val,uint8_t byt);
    uint8_t msb_pos(uint64_t val); // returns the position of most significant bit
};

#endif // define ADS126X_H
