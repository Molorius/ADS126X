ADS126X
=======
A class to control the TI ADS1262 and ADS1263 analog to digital converters.
This basically includes all functions of these two chips.
Check the examples if it seems too complicated.


Note that all commands dealing with ACD2 are for the ADS1263 only, and have not
been tested by me.

I tried to keep all commands in the order the [datasheet](http://www.ti.com/lit/ds/symlink/ads1262.pdf) is written in for ease of reading.
Basic commands are on page 85, the register map is on page 88.

Table of Contents
=================
* [Initialization](#initialization)
   * [ADS126X](#ads126x "ADS126X")
   * [begin(chip_select)](#void-beginuint8_t-chip_select "void begin(uint8_t chip_select)")
   * [begin](#void-begin "void begin()")
   * [setStartPin](#void-setstartpinuint8_t-pin "void setStartPin(uint8_t pin)")
* [General Commands](#general-commands)
   * [noOperation](#void-noOperation "void noOperation()")
   * [reset](#void-reset "void reset()")
   * [startADC1](#void-startadc1 "void startADC1()")
   * [stopADC1](#void-stopadc1 "void stopADC1()")
   * [startADC2](#void-startadc2 "void startADC2()")
   * [stopADC2](#void-stopadc2 "void stopADC2()")
* [Analog Read Functions](#analog-read-functions)
   * [readADC1](#int32_t-readadc1uint8_t-pos_pinuint8_t-neg_pin "int32_t readADC1(uint8_t pos_pin,uint8_t neg_pin)")
   * [readADC2](#int32_t-readadc2uint8_t-pos_pinuint8_t-neg_pin "int32_t readADC2(uint8_t pos_pin,uint8_t neg_pin)")   
* [Calibration Functions](#calibration-functions)
   * [calibrateSysOffsetADC1](#void-calibratesysoffsetadc1uint8_t-shorted1uint8_t-shorted2 "void calibrateSysOffsetADC1(uint8_t shorted1,uint8_t shorted2)")
   * [calibrateGainADC1](#void-calibrategainadc1uint8_t-vcc_pinuint8_t-gnd_pin "void calibrateGainADC1(uint8_t vcc_pin,uint8_t gnd_pin)")
   * [calibrateSelfOffsetADC1](#void-calibrateselfoffsetadc1 "void calibrateSelfOffsetADC1(void)")
   * [calibrateSysOffsetADC2](#void-calibratesysoffsetadc2uint8_t-shorted1uint8_t-shorted2 "void calibrateSysOffsetADC2(uint8_t shorted1,uint8_t shorted2)")
   * [calibrateGainADC2](#void-calibrategainadc2uint8_t-vcc_pinuint8_t-gnd_pin "void calibrateGainADC2(uint8_t vcc_pin,uint8_t gnd_pin)")
   * [calibrateSelfOffsetADC2](#void-calibrateselfoffsetadc2 "void calibrateSelfOffsetADC2(void)")
* [IDAC Functions](#idac-functions)
   * [setIDAC1Pin](#void-setidac1pinuint8_t-pin "void setIDAC1Pin(uint8_t pin)")
   * [setIDAC2Pin](#void-setidac2pinuint8_t-pin "void setIDAC2Pin(uint8_t pin)")
   * [setIDAC1Mag](#void-setidac1maguint8_t-magnitude "void setIDAC1Mag(uint8_t magnitude)")
   * [setIDAC2Mag](#void-setidac2maguint8_t-magnitude "void setIDAC2Mag(uint8_t magnitude)")
* [Power Functions](#power-functions)
   * [checkResetBit](#bool-checkresetbit "bool checkResetBit()")
   * [clearResetBit](#void-clearresetbit "void clearResetBit()")
   * [enableLevelShift](#void-enablelevelshift "void enableLevelShift()")
   * [disableLevelShift](#void-disablelevelshift "void disableLevelShift()")
   * [enableInternalReference](#void-enableinternalreference "void enableInternalReference()")
   * [disableInternalReference](#void-disableinternalreference "void disableInternalReference()")
* [Checksum Functions](#checksum-functions)
   * [disableCheck](#void-disablecheck "void disableCheck()")
   * [setChecksumMode](#void-setchecksummode "void setChecksumMode()")
   * [setCRCMode](#void-setcrcmode "void setCRCMode()")
   * [lastChecksum](#bool-lastchecksum "bool lastChecksum()")
* [Status Functions](#status-functions)
   * [enableStatus](#void-enablestatus "void enableStatus()")
   * [disableStatus](#void-disablestatus "void disableStatus()")
   * [lastStatus](#uint8_t-laststatus "uint8_t lastStatus()")
   * [lastADC2Status](#bool-lastadc2status "bool lastADC2Status()")
   * [lastADC1Status](#bool-lastadc2status "bool lastADC1Status()")
   * [lastClockSource](#bool-lastclocksource "bool lastClockSource()")
   * [lastADC1LowReferenceAlarm](#bool-lastadc1lowreferencealarm "bool lastADC1LowReferenceAlarm()")
   * [lastADC1PGAOutputLowAlarm](#bool-lastadc1pgaoutputlowalarm "bool lastADC1PGAOutputLowAlarm()")
   * [lastADC1PGAOutputHighAlarm](#bool-lastadc1pgaoutputhighalarm "bool lastADC1PGAOutputHighAlarm()")
   * [lastADC1PGADifferentialOutputAlarm](#bool-lastadc1pgadifferentialoutputalarm "bool lastADC1PGADifferentialOutputAlarm()")
   * [lastReset](#bool-lastreset "bool lastReset()")
* [Mode0 Functions](#mode0-functions)
   * [setContinuousMode](#void-setcontinuousmode "void setContinousMode()")
   * [setPulseMode](#void-setpulsemode "void setPulseMode()")
   * [setChopMode](#void-setchopmodeuint8_t-mode "void setChopMode(uint8_t mode)")
   * [setDelay](#void-setDelayuint8_t-del "void setDelay(uint8_t del)")
* [Mode1 Functions](#mode1-functions)
   * [setFilter](#void-setfilteruint8_t-filter "void setFilter(uint8_t filter)")
   * [setBiasADC](#void-setbiasadcuint8_t-adc_choice "void setBiasADC(uint8_t adc_choice)")
   * [setBiasPolarity](#void-setbiaspolarityuint8_t-polarity "void setBiasPolarity(uint8_t polarity)")
   * [setBiasMagnitude](#void-setbiasmagnitudeuint8_t-mag "void setBiasMagnitude(uint8_t mag)")
* [Mode2 Functions](#mode2-functions)
   * [enablePGA](#void-enablepga "void enablePGA()")
   * [bypassPGA](#void-bypasspga "void bypassPGA()")
   * [setGain](#void-setgainuint8_t-gain "void setGain(uint8_t gain)")
   * [setRate](#void-setrateuint8_t-rate "void setRate(uint8_t rate)")
   * [setReference](#void-setreferenceuint8_t-negativereference-uint8_t-positivereference "void setReference(uint8_t negativeReference, uint8_t positiveReference)")
* [GPIO Functions](#gpio-functions)
   * [gpioConnect](#void-gpioconnectuint8_t-pin "void gpioConnect(uint8_t pin)")
   * [gpioDisconnect](#void-gpiodisconnectuint8_t-pin "void gpioDisconnect(uint8_t pin)")
   * [gpioDirection](#void-gpiodirectionuint8_t-pinuint8_t-direction "void gpioDirection(uint8_t pin,uint8_t direction)")
   * [gpioWrite](#void-gpiowriteuint8_t-pinuint8_t-val "void gpioWrite(uint8_t pin,uint8_t val)")
   * [gpioRead](#bool-gpioreaduint8_t-pin "bool gpioRead(uint8_t pin)")

Initialization
==============

ADS126X
-------
Starts the class.

void begin(uint8_t chip_select)
-------------------------------
Sets up the chip and Arduino for usage. It also resets the chip.
`uint8_t chip_select` is the Arduino pin used to start communication with the chip.

void begin()
------------
The same as the above function, but without the chip select.
If you use this, tie the CS pin on the chip to ground.
I don't recommend using this.

void setStartPin(uint8_t pin)
-----------------------------
Optional. Designate an Arduino pin to start ADC1.
If not used, a command will be sent instead.

void setDrdyPin(uint8_t pin)
-----------------------------
Optional. Designate an Arduino pin to watch for Data Ready.
If not used, calibrations will require a `waitTime` and dataReady() will always return true.

General Commands
================

void noOperation()
------------------
Tells the chip to stop everything. Future commands will wake it back up.

void reset()
------------
Resets the chip.

void startADC1(uint8_t pos_pin, uint8_t neg_pin)
----------------
Starts conversion on ADC1 between the two pins `pos_pin` and `neg_pin`.
These can be:

|        Option         | Description                                    |
|-----------------------|------------------------------------------------|
| `0` or `ADS126X_AIN0` | Pin AIN0                                       |
|          ...          |    ...                                         |
| `9` or `ADS126X_AIN9` | Pin AIN9                                       |
|   `ADS126X_AINCOM`    | Pin AINCOM                                     |
|     `ADS126X_TEMP`    | Temperature sensor monitor positive/negative   |
|    `ADS126X_ANALOG`   | Analog power supply monitor positive/negative  |
|   `ADS126X_DIGITAL`   | Digital power supply monitor positive/negative |
|     `ADS126X_TDAC`    | TDAC test signal positive/negative             |
|    `ADS126X_FLOAT`    | Float (open connection)                        |

void stopADC1()
---------------
Stops conversion on ADC1. Uses `start_pin` if it was set (via `setStartPin`), sends a command if it wasn't.

void startADC2(uint8_t pos_pin, uint8_t neg_pin)
----------------
Starts conversion on ADC2 between the two pins `pos_pin` and `neg_pin`.
These can be:

|        Option         | Description                                    |
|-----------------------|------------------------------------------------|
| `0` or `ADS126X_AIN0` | Pin AIN0                                       |
|          ...          |    ...                                         |
| `9` or `ADS126X_AIN9` | Pin AIN9                                       |
|   `ADS126X_AINCOM`    | Pin AINCOM                                     |
|     `ADS126X_TEMP`    | Temperature sensor monitor positive/negative   |
|    `ADS126X_ANALOG`   | Analog power supply monitor positive/negative  |
|   `ADS126X_DIGITAL`   | Digital power supply monitor positive/negative |
|     `ADS126X_TDAC`    | TDAC test signal positive/negative             |
|    `ADS126X_FLOAT`    | Float (open connection)                        |  

void stopADC2()
---------------
Stops conversion on ADC2.  

Analog Read Functions
=====================

int32_t readADC1()
-------------------------------------------------
Reads the 32 bit voltage between the pins set in `startADC1`.

int32_t readADC2(uint8_t pos_pin,uint8_t neg_pin)
-------------------------------------------------
Reads the 24 bit voltage between the pins set in `startADC2`.

bool dataReady()
---------------
If `setDrdyPin` was used, returns true when the adc's `DRDY` (data ready indicator) pin is `LOW`. If not pin was set, this always returns true. Useful for getting continuous conversions as quickly as possible. Almost certainly faster than command polling (especially if you attach this pin to an interrupt routine).

Calibration Functions
=====================

void calibrateSysOffsetADC1(uint8_t shorted1,uint8_t shorted2)
--------------------------------------------------------------
Calibrates the system offset on ADC1. Short the pins externally, then run this command.
Options can be:

|        Option         | Description |
|-----------------------|-------------|
| `0` or `ADS126X_AIN0` | Pin AIN0    |
|          ...          |    ...      |
| `9` or `ADS126X_AIN9` | Pin AIN9    |

void calibrateGainADC1(uint8_t vcc_pin,uint8_t gnd_pin)
-------------------------------------------------------
Calibrates the PGA on ADC1.
`vcc_pin` should be connected to the maximum analog voltage,`gnd_pin` to analog ground.
Options can be:

|        Option         | Description |
|-----------------------|-------------|
| `0` or `ADS126X_AIN0` | Pin AIN0    |
|          ...          |    ...      |
| `9` or `ADS126X_AIN9` | Pin AIN9    |

void calibrateSelfOffsetADC1()
------------------------------
Calibrates the self offset on ADC1.

void calibrateSysOffsetADC2(uint8_t shorted1,uint8_t shorted2)
--------------------------------------------------------------
Calibrates the system offset on ADC2. Short the pins externally, then run this command.
Options can be:

|        Option         | Description |
|-----------------------|-------------|
| `0` or `ADS126X_AIN0` | Pin AIN0    |
|          ...          |    ...      |
| `9` or `ADS126X_AIN9` | Pin AIN9    |

void calibrateGainADC2(uint8_t vcc_pin,uint8_t gnd_pin)
-------------------------------------------------------
Calibrates the PGA on ADC2.
`vcc_pin` should be connected to the maximum analog voltage,`gnd_pin` to analog ground.
Options can be:

|        Option         | Description |
|-----------------------|-------------|
| `0` or `ADS126X_AIN0` | Pin AIN0    |
|          ...          |    ...      |
| `9` or `ADS126X_AIN9` | Pin AIN9    |

void calibrateSelfOffsetADC2()
------------------------------
Calibrates the self offset on ADC2.

IDAC Functions
==============
Functions relating to IDAC1 and IDAC2.

void setIDAC1Pin(uint8_t pin)
-----------------------------
Selects the analog input pin to connect IDAC1.
Options can be:

|           Option           |        Description      |
|----------------------------|-------------------------|
| `0` or `ADS126X_IDAC_AIN0` |          Pin AIN0       |
|            ...             |            ...          |
| `9` or `ADS126X_IDAC_AIN9` |          Pin AIN9       |
|    `ADS126X_IDAC_AINCOM`   |         Pin AINCOM      |
|      `ADS126X_IDAC_NC`     | No Connection (default) |

void setIDAC2Pin(uint8_t pin)
-----------------------------
Selects the analog input pin to connect IDAC2.
Options can be:

|           Option           |        Description      |
|----------------------------|-------------------------|
| `0` or `ADS126X_IDAC_AIN0` |          Pin AIN0       |
|            ...             |            ...          |
| `9` or `ADS126X_IDAC_AIN9` |          Pin AIN9       |
|    `ADS126X_IDAC_AINCOM`   |         Pin AINCOM      |
|      `ADS126X_IDAC_NC`     | No Connection (default) |

void setIDAC1Mag(uint8_t magnitude)
-----------------------------------
Selects the current values of IDAC1.
Options can be:

|          Option         |  Description  |
|-------------------------|---------------|
|   `ADS126X_IDAC_MAG_0`  | off (default) |
|  `ADS126X_IDAC_MAG_50`  |     50 µA     |
| `ADS126X_IDAC_MAG_100`  |    100 µA     |
| `ADS126X_IDAC_MAG_250`  |    250 µA     |
| `ADS126X_IDAC_MAG_500`  |    500 µA     |
| `ADS126X_IDAC_MAG_750`  |    750 µA     |
| `ADS126X_IDAC_MAG_1000` |   1000 µA     |
| `ADS126X_IDAC_MAG_1500` |   1500 µA     |
| `ADS126X_IDAC_MAG_2000` |   2000 µA     |
| `ADS126X_IDAC_MAG_2500` |   2500 µA     |
| `ADS126X_IDAC_MAG_3000` |   3000 µA     |

void setIDAC2Mag(uint8_t magnitude)
-----------------------------------
Selects the current values of IDAC2.
Options can be:

|          Option         |  Description  |
|-------------------------|---------------|
|   `ADS126X_IDAC_MAG_0`  | off (default) |
|  `ADS126X_IDAC_MAG_50`  |     50 µA     |
| `ADS126X_IDAC_MAG_100`  |    100 µA     |
| `ADS126X_IDAC_MAG_250`  |    250 µA     |
| `ADS126X_IDAC_MAG_500`  |    500 µA     |
| `ADS126X_IDAC_MAG_750`  |    750 µA     |
| `ADS126X_IDAC_MAG_1000` |   1000 µA     |
| `ADS126X_IDAC_MAG_1500` |   1500 µA     |
| `ADS126X_IDAC_MAG_2000` |   2000 µA     |
| `ADS126X_IDAC_MAG_2500` |   2500 µA     |
| `ADS126X_IDAC_MAG_3000` |   3000 µA     |

Power Functions
===============
I didn't know what else to call this category.
All functions have to do with the POWER register.

bool checkResetBit()
-----------------------
Indicates ADC reset has occurred. If 1, a new reset has occured.
This should be 1 at start.

void clearResetBit()
--------------------
Sets the reset bit to 0.

void enableLevelShift()
-----------------------
Enables the internal level shift voltage to the AINCOM pin.
Default disabled.
VBIAS = (V_AVDD + V_AVSS)/2

void disableLevelShift()
------------------------
Disables the internal level shift voltage to the AINCOM pin.
Default disabled.
VBIAS = (V_AVDD + V_AVSS)/2

void enableInternalReference()
------------------------------
Enables the 2.5V internal voltage reference.
Note the IDAC and temperature sensor require the internal voltage reference.
Enabled by default.

void disableInternalReference()
------------------------------
Disables the 2.5V internal voltage reference.
Note the IDAC and temperature sensor require the internal voltage reference.
Enabled by default.


Checksum Functions
==================
If enabled, the chip will send a checksum during a voltage read to verify the data
was sent correctly. These functions will set the mode, and return the verification.

void disableCheck()
-------------------
Disable the checksum.

void setChecksumMode()
----------------------
Make the device return a checksum value.

void setCRCMode()
-----------------
Make the device return a CRC value.

bool lastChecksum()
----------------------
Return the computed last checksum/CRC value.
If 0, the data was transmitted correctly.


Status Functions
================
If enabled, the chip will send the status of various parameters as part of each voltage read.
All functions below will use the most recent status sent from the chip.

void enableStatus()
-------------------
Enables the chip sending the status. Default enabled.

void disableStatus()
--------------------
Disables the chip sending the status.

uint8_t lastStatus()
--------------------
Returns the most recent status byte. More for debugging than anything.

bool lastADC2Status()
------------------------
If 1, ADC2 data is new since the last ADC2 read operation.

bool lastADC1Status()
------------------------
If 1, ADC1 data is new since the last ADC1 read operation.

bool lastClockSource()
-------------------------
If 0, ADC clock is internal.
If 1, ADC clock is external.

bool lastADC1LowReferenceAlarm()
-----------------------------------
This is the low reference voltage alarm of ADC1.
The alarm bit is set if V_REF <= 0.4V, typical.
If 0, no alarm.
If 1, low reference alarm.

bool lastADC1PGAOutputLowAlarm()
-----------------------------------
This is the ADC1 PGA absolute low voltage alarm.
The bit is set if the absolute voltage of either PGA output is less than
V_AVSS + 0.2V. See the PGA Absolute Output-Voltage Monitor section of the datasheet.
If 0, no alarm
If 1, PGA high voltage alarm

bool lastADC1PGAOutputHighAlarm()
------------------------------------
This is the ADC1 PGA absolute high voltage alarm.
The bit is set if the absolute voltage of either PGA output is greater than
V_AVDD - 0.2V. See the PGA Absolute Output-Voltage Monitor section of the datasheet.
If 0, no alarm
If 1, PGA high voltage alarm

bool lastADC1PGADifferentialOutputAlarm()
--------------------------------------------
This is the ADC1 PGA differential output range alarm.
This bit is set if the PGA differential output voltage exceeds +105% FS or -105% FS.
See the PGA Differential Output Monitor section of the datasheet.
If 0, no alarm
If 1, PGA differential range alarm

bool lastReset()
-------------------
Indicates device reset. Device reset occurs at power-on, by the RESET/PWDN pin
or by the reset command. This bit is the same as the RESET bit of the POWER register.
If 0, no reset occurred since the RESET bit in power register last cleared by the user.
If 1, device reset occurred


Mode0 Functions
===============
Functions having to do with the MODE0 register.

void setContinuousMode()
------------------------
Puts the device into continuous read mode. Default.

void setPulseMode()
-------------------
Puts the device into pulse read mode.
Normally you would need to call the startADC* function before every readADC*
in this mode, but I have it automatically called.

void setChopMode(uint8_t mode)
------------------------------
Enables the ADC chop and ICAD rotation options.

|      Option      | Description                                     |
|------------------|-------------------------------------------------|
| `ADS126X_CHOP_0` | Input chop and IDAC rotation disabled (default) |
| `ADS126X_CHOP_1` | Input chop enabled                              |
| `ADS126X_CHOP_2` | IDAC rotaion enabled                            |
| `ADS126X_CHOP_3` | Input chop and IDAC rotation enabled            |

void setDelay(uint8_t del)
--------------------------
Provides additional delay from conversion start to the beginning
of the actual conversion. Default no delay.

|          Option        |  Delay   |
|------------------------|----------|
|   `ADS126X_DELAY_0`    | No delay |
|  `ADS126X_DELAY_8700`  |  8.7 s   |
|  `ADS126X_DELAY_17000` |   17 s   |
|  `ADS126X_DELAY_35000` |   35 s   |
|  `ADS126X_DELAY_69000` |   69 s   |
| `ADS126X_DELAY_139000` |  139 s   |
| `ADS126X_DELAY_278000` |  278 s   |
| `ADS126X_DELAY_555000` |  555 s   |
|  `ADS126X_DELAY_1_1`   |  1.1 ms  |
|  `ADS126X_DELAY_2_2`   |  2.2 ms  |
|  `ADS126X_DELAY_4_4`   |  4.4 ms  |
|  `ADS126X_DELAY_8_8`   |  8.8 ms  |


Mode1 Functions
===============
Functions having to do with the MODE1 register.

void setFilter(uint8_t filter)
------------------------------
Configures the ADC digital filter.
Detailed description can be found starting on page 45.

|     Option      | Description        |
|-----------------|--------------------|
| `ADS126X_SINC1` | Sinc1 mode         |
| `ADS126X_SINC2` | Sinc2 mode         |
| `ADS126X_SINC3` | Sinc3 mode         |
| `ADS126X_SINC4` | Sinc4 mode         |
|  `ADS126X_FIR`  | FIR mode (default) |

void setBiasADC(uint8_t adc_choice)
-----------------------------------
Selects the ADC to connect the sensor bias.

|           Option           | Description                                     |
|----------------------------|-------------------------------------------------|
| `0` or `ADS126X_BIAS_ADC1` | Sensor bias connected to ADC1 mux out (default) |
| `1` or `ADS126X_BIAS_ADC2` | Sensor bias connected to ADC2 mux out           |

void setBiasPolarity(uint8_t polarity)
--------------------------------------
Selects the sensor bias polarity for pull-up or pull-down

|             Option             | Sensor bias pull-up mode                      |
|--------------------------------|-----------------------------------------------|
|  `0` or `ADS126X_BIAS_PULLUP`  | AIN_P pulled high, AIN_N pulled low (default) |
| `1` or `ADS126X_BIAS_PULLDOWN` | AIN_P pulled low, AIN_N pulled high           |

void setBiasMagnitude(uint8_t mag)
----------------------------------
Selects the sensor bias current magnitude or the bias resistor

|         Option         | Description                                  |
|------------------------|----------------------------------------------|
|  `ADS126X_BIAS_MAG_0`  | No sensor bias current or resistor (default) |
| `ADS126X_BIAS_MAG_0_5` | 0.5 A sensor bias current                    |
|  `ADS126X_BIAS_MAG_2`  | 2 A sensor bias current                      |
| `ADS126X_BIAS_MAG_10`  | 10 A sensor bias current                     |
| `ADS126X_BIAS_MAG_50`  | 50 A sensor bias current                     |
| `ADS126X_BIAS_MAG_200` | 200 A sensor bias current                    |
| `ADS126X_BIAS_MAG_10M` | 10 MOhm resistor                             |


Mode2 Functions
===============

void enablePGA()
----------------
Enables the PGA, default

void bypassPGA()
----------------
PGA bypassed

void setGain(uint8_t gain)
--------------------------
Selects the PGA gain

|       Option      | Gain   |
|-------------------|--------|
| `ADS126X_GAIN_1`  | 1 V/V  |
| `ADS126X_GAIN_2`  | 2 V/V  |
| `ADS126X_GAIN_4`  | 4 V/V  |
| `ADS126X_GAIN_8`  | 8 V/V  |
| `ADS126X_GAIN_16` | 16 V/V |
| `ADS126X_GAIN_32` | 32 V/V |

void getGain()
--------------
Returns the value of the constant used in setGain (i.e. `ADS126X_GAIN_1` -> `0b000` which is int `0`). To get the corresponding actual voltage gain, do something like `uint8_t voltageGainValue = 1 << adc.getGain();` (`ADS126X_GAIN_1` would return `1`).

void setRate(uint8_t rate)
--------------------------
Selects the ADC data rate. In FIR filter mode, the available data
rates are limited to 2.5, 5, 10, and 20 FPS. This class does NOT
make sure you set that up right.

|        Option        | Speed     |
|----------------------|-----------|
|  `ADS126X_RATE_2_5`  | 2.5 SPS   |
|   `ADS126X_RATE_5`   | 5 SPS     |
|   `ADS126X_RATE_10`  | 10 SPS    |
|  `ADS126X_RATE_16_6` | 16.6 SPS  |
|   `ADS126X_RATE_20`  | 20 SPS    |
|   `ADS126X_RATE_50`  | 50 SPS    |
|   `ADS126X_RATE_60`  | 60 SPS    |
|  `ADS126X_RATE_100`  | 100 SPS   |
|  `ADS126X_RATE_400`  | 400 SPS   |
| `ADS126X_RATE_1200`  | 1200 SPS  |
| `ADS126X_RATE_2400`  | 2400 SPS  |
| `ADS126X_RATE_4800`  | 4800 SPS  |
| `ADS126X_RATE_7200`  | 7200 SPS  |
| `ADS126X_RATE_14400` | 14400 SPS |
| `ADS126X_RATE_19200` | 19200 SPS |
| `ADS126X_RATE_38400` | 38400 SPS |

void setReference(uint8_t negativeReference, uint8_t positiveReference)
--------------------------
Selects the positive and negative references.

|    Option - Negative    | Reference |
|-------------------------|-----------|
|  `ADS126X_REF_NEG_INT`  | INTERNAL  |
|  `ADS126X_REF_NEG_AIN1` | AIN1      |
|  `ADS126X_REF_NEG_AIN3` | AIN3      |
|  `ADS126X_REF_NEG_AIN5` | AIN5      |
|  `ADS126X_REF_NEG_VSS`  | VSS       |

|    Option - Positive    | Reference |
|-------------------------|-----------|
|  `ADS126X_REF_POS_INT`  | INTERNAL  |
|  `ADS126X_REF_POS_AIN0` | AIN0      |
|  `ADS126X_REF_POS_AIN2` | AIN2      |
|  `ADS126X_REF_POS_AIN4` | AIN4      |
|  `ADS126X_REF_POS_VDD`  | VDD       |



GPIO Functions
==============
These all pertain to the gpio pins on the chip.

For all of them, there is an input `uint8_t pin`.
This is the gpio pin that you want to use. Map:

| PinCode (`uint8_t pin`) | External Pin |
|-------------------------|--------------|
|           `0`           | AIN3         |
|           `1`           | AIN4         |
|           `2`           | AIN5         |
|           `3`           | AIN6         |
|           `4`           | AIN7         |
|           `5`           | AIN8         |
|           `6`           | AIN9         |
|           `7`           | AINCOM       |

void gpioConnect(uint8_t pin)
-----------------------------
Connect the gpio pin to the external pin

void gpioDisconnect(uint8_t pin)
--------------------------------
Disconnect the gpio pin from the external pin

void gpioDirection(uint8_t pin,uint8_t direction)
-------------------------------------------------
Configures the pin as output or input.
All are set to output by default.

|            Option            | Description             |
|------------------------------|-------------------------|
| `0` or `ADS126X_GPIO_OUTPUT` | Configure pin as output |
| `1` or `ADS126X_GPIO_INPUT`  | Configure pin as input  |

void gpioWrite(uint8_t pin,uint8_t val)
---------------------------------------
Writes a value to a pin configured as output.

|           Option           | Description  |
|----------------------------|--------------|
| `0` or `ADS126X_GPIO_LOW`  | Set pin low  |
| `1` or `ADS126X_GPIO_HIGH` | Set pin high |

bool gpioRead(uint8_t pin)
--------------------------
Reads the value of a pin configured as input.

Returns 0 if low
Returns 1 if high
