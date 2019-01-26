
#ifndef ADS126X_DEFINITIONS_H
#define ADS126X_DEFINITIONS_H

#ifdef __cplusplus
extern "C" {
#endif

# ifndef __IO
#define __IO volatile
# endif

// commands. Page 85, table 37.
#define ADS126X_NOP               0x00
#define ADS126X_RESET             0x06
#define ADS126X_START1            0x08
#define ADS126X_STOP1             0x0A
#define ADS126X_START2            0x0C
#define ADS126X_STOP2             0x0E
#define ADS126X_RDATA1            0x12
#define ADS126X_RDATA2            0x14
#define ADS126X_SYOCAL1           0x16
#define ADS126X_SYGCAL1           0x17
#define ADS126X_SFOCAL1           0x19
#define ADS126X_SYOCAL2           0x1B
#define ADS126X_SYGCAL2           0x1C
#define ADS126X_SFOCAL2           0x1E
#define ADS126X_RREG              0x20
#define ADS126X_WREG              0x40

//register addresses. Page 88, table 38.
#define ADS126X_ID                0x00
#define ADS126X_POWER             0x01
#define ADS126X_INTERFACE         0x02
#define ADS126X_MODE0             0x03
#define ADS126X_MODE1             0x04
#define ADS126X_MODE2             0x05
#define ADS126X_INPMUX            0x06
#define ADS126X_OFCAL0            0x07
#define ADS126X_OFCAL1            0x08
#define ADS126X_OFCAL2            0x09
#define ADS126X_FSCAL0            0x0A
#define ADS126X_FSCAL1            0x0B
#define ADS126X_FSCAL2            0x0C
#define ADS126X_IDACMUX           0x0D
#define ADS126X_IDACMAG           0x0E
#define ADS126X_REFMUX            0x0F
#define ADS126X_TDACP             0x10
#define ADS126X_TDACN             0x11
#define ADS126X_GPIOCON           0x12
#define ADS126X_GPIODIR           0x13
#define ADS126X_GPIODAT           0x14
#define ADS126X_ADC2CFG           0x15
#define ADS126X_ADC2MUX           0x16
#define ADS126X_ADC2OFC0          0x17
#define ADS126X_ADC2OFC1          0x18
#define ADS126X_ADC2FSC0          0x19
#define ADS126X_ADC2FSC1          0x1A

#define ADS126X_REG_NUM           0x1B // number of registers

// random values from the datasheet
#define ADS126X_CHECK_BYTE        0x9B // used to calculate checksum
#define ADS126X_CRC_BYTE          0b100000111
#define ADS126X_DUMMY             0x00 // dummy for spi reads

#define ADS126X_ENABLE            0x01 // used throughout the registers
#define ADS126X_DISABLE           0x00

// checksum byte. Page 90.
#define ADS126X_CHECKSUM          0b01
#define ADS126X_CHECK_CRC         0b10

// MODE0 values. Page 91
#define ADS126X_POL_NORM          0b0 // reverses the ADC1 reference multiplexer output polarity
#define ADS126X_POL_REV           0b1
#define ADS126X_CONV_CONT         0b0 // adc conversion run mode
#define ADS126X_CONV_PULSE        0b1
#define ADS126X_CHOP_0            0b00 // enables the ADC chop and IDAC rotation options
#define ADS126X_CHOP_1            0b01
#define ADS126X_CHOP_2            0b10
#define ADS126X_CHOP_3            0b11
#define ADS126X_DELAY_0           0b0000 // conversion delay in milliseconds
#define ADS126X_DELAY_8700        0b0001
#define ADS126X_DELAY_17000       0b0010
#define ADS126X_DELAY_35000       0b0011
#define ADS126X_DELAY_69000       0b0100
#define ADS126X_DELAY_139000      0b0101
#define ADS126X_DELAY_278000      0b0110
#define ADS126X_DELAY_555000      0b0111
#define ADS126X_DELAY_1_1         0b1000
#define ADS126X_DELAY_2_2         0b1001
#define ADS126X_DELAY_4_4         0b1010
#define ADS126X_DELAY_8_8         0b1011

// MODE1 values. Page 92
#define ADS126X_SINC1             0b000 // configures the ADC digital filter
#define ADS126X_SINC2             0b001
#define ADS126X_SINC3             0b010
#define ADS126X_SINC4             0b011
#define ADS126X_FIR               0b100
#define ADS126X_BIAS_ADC1         0b0 // selects the ADC to connect the sensor bias
#define ADS126X_BIAS_ADC2         0b1
#define ADS126X_BIAS_PULLUP       0b0 // selects the sensor bias for pull-up or pull-down
#define ADS126X_BIAS_PULLDOWN     0b1
#define ADS126X_BIAS_MAG_0        0b000 // Selects the sensor bias current magnitude or the bias resistor
#define ADS126X_BIAS_MAG_0_5      0b001
#define ADS126X_BIAS_MAG_2        0b010
#define ADS126X_BIAS_MAG_10       0b011
#define ADS126X_BIAS_MAG_50       0b100
#define ADS126X_BIAS_MAG_200      0b101
#define ADS126X_BIAS_MAG_10M      0b110

// MODE2 values. Page 93
#define ADS126X_PGA_ENABLE        0b0 // pga bypass mode, different than other enable and disable
#define ADS126X_PGA_BYPASS        0b1
#define ADS126X_GAIN_1            0b000 // selects the PGA gain
#define ADS126X_GAIN_2            0b001
#define ADS126X_GAIN_4            0b010
#define ADS126X_GAIN_8            0b011
#define ADS126X_GAIN_16           0b100
#define ADS126X_GAIN_32           0b101
#define ADS126X_RATE_2_5          0b0000 // selects the ADC data rate in SPS. In FIR only 2.5, 5, 10, and 20
#define ADS126X_RATE_5            0b0001
#define ADS126X_RATE_10           0b0010
#define ADS126X_RATE_16_6         0b0011
#define ADS126X_RATE_20           0b0100
#define ADS126X_RATE_50           0b0101
#define ADS126X_RATE_60           0b0110
#define ADS126X_RATE_100          0b0111
#define ADS126X_RATE_400          0b1000
#define ADS126X_RATE_1200         0b1001
#define ADS126X_RATE_2400         0b1010
#define ADS126X_RATE_4800         0b1011
#define ADS126X_RATE_7200         0b1100
#define ADS126X_RATE_14400        0b1101
#define ADS126X_RATE_19200        0b1110
#define ADS126X_RATE_38400        0b1111

// pins to connect to. Page 94. Note that pins 0-9 correspond to the decimal values.
#define ADS126X_AIN0              0b0000
#define ADS126X_AIN1              0b0001
#define ADS126X_AIN2              0b0010
#define ADS126X_AIN3              0b0011
#define ADS126X_AIN4              0b0100
#define ADS126X_AIN5              0b0101
#define ADS126X_AIN6              0b0110
#define ADS126X_AIN7              0b0111
#define ADS126X_AIN8              0b1000
#define ADS126X_AIN9              0b1001
#define ADS126X_AINCOM            0b1010
#define ADS126X_TEMP              0b1011
#define ADS126X_ANALOG            0b1100
#define ADS126X_DIGITAL           0b1101
#define ADS126X_TDAC              0b1110
#define ADS126X_FLOAT             0b1111

// IDAC Output pins. Page 96. Note that pins 0-9 correspond to the decimal values.
#define ADS126X_IDAC_AIN0         0b0000
#define ADS126X_IDAC_AIN1         0b0001
#define ADS126X_IDAC_AIN2         0b0010
#define ADS126X_IDAC_AIN3         0b0011
#define ADS126X_IDAC_AIN4         0b0100
#define ADS126X_IDAC_AIN5         0b0101
#define ADS126X_IDAC_AIN6         0b0110
#define ADS126X_IDAC_AIN7         0b0111
#define ADS126X_IDAC_AIN8         0b1000
#define ADS126X_IDAC_AIN9         0b1001
#define ADS126X_IDAC_AINCOM       0b1010
#define ADS126X_IDAC_NC           0b1011

// IDAC Magnitute. Page 97
#define ADS126X_IDAC_MAG_0        0b0000
#define ADS126X_IDAC_MAG_50       0b0001
#define ADS126X_IDAC_MAG_100      0b0010
#define ADS126X_IDAC_MAG_250      0b0011
#define ADS126X_IDAC_MAG_500      0b0100
#define ADS126X_IDAC_MAG_750      0b0101
#define ADS126X_IDAC_MAG_1000     0b0110
#define ADS126X_IDAC_MAG_1500     0b0111
#define ADS126X_IDAC_MAG_2000     0b1000
#define ADS126X_IDAC_MAG_2500     0b1001
#define ADS126X_IDAC_MAG_3000     0b1010

// Reference Multiplexer. Page 98
#define ADS126X_REF_POS_INT       0b000 // positive
#define ADS126X_REF_POS_AIN0      0b001
#define ADS126X_REF_POS_AIN2      0b010
#define ADS126X_REF_POS_AIN4      0b011
#define ADS126X_REF_POS_VDD       0b100
#define ADS126X_REF_NEG_INT       0b000 // negative
#define ADS126X_REF_NEG_AIN1      0b001
#define ADS126X_REF_NEG_AIN3      0b010
#define ADS126X_REF_NEG_AIN5      0b011
#define ADS126X_REF_NEG_VSS       0b100

// TDACP Output. Page 99
#define ADS126X_TDACP_NC          0b0 // connects TDACP to pin AIN6
#define ADS126X_TDACP_AIN6        0b1

// TDACN Output. Page 100
#define ADS126X_TDACN_NC          0b0 // connects TDACN to pin AIN7
#define ADS126X_TDACN_AIN7        0b1

// TDAC output magnitude. The value is V*{divider ratio}. Pages _54_, 99, 100
#define ADS126X_TDAC_DIV_0_9      0b01001
#define ADS126X_TDAC_DIV_0_7      0b01000
#define ADS126X_TDAC_DIV_0_6      0b00111
#define ADS126X_TDAC_DIV_0_55     0b00110
#define ADS126X_TDAC_DIV_0_525    0b00101
#define ADS126X_TDAC_DIV_0_5125   0b00100
#define ADS126X_TDAC_DIV_0_50625  0b00011
#define ADS126X_TDAC_DIV_0_503125 0b00010
#define ADS126X_TDAC_DIV_0_5015625 0b00001
#define ADS126X_TDAC_DIV_0_5       0b00000
#define ADS126X_TDAC_DIV_0_4984375 0b10001
#define ADS126X_TDAC_DIV_0_496875 0b10010
#define ADS126X_TDAC_DIV_0_49375  0b10011
#define ADS126X_TDAC_DIV_0_4875   0b10100
#define ADS126X_TDAC_DIV_0_475    0b10101
#define ADS126X_TDAC_DIV_0_45     0b10110
#define ADS126X_TDAC_DIV_0_4      0b10111
#define ADS126X_TDAC_DIV_0_3      0b11000
#define ADS126X_TDAC_DIV_0_1      0b11001

// gpio stuff. Pages 101-103
#define ADS126X_GPIO_DISCONNECT   0b0 // connect or disconnect gpio pins
#define ADS126X_GPIO_CONNECT      0b1
#define ADS126X_GPIO_OUTPUT       0b0 // output or input
#define ADS126X_GPIO_INPUT        0b1
#define ADS126X_GPIO_LOW          0b0 // low or high in/out
#define ADS126X_GPIO_HIGH         0b1

// ADC2 Configuration
#define ADS126X_ADC2_RATE_10      0b00 // data rate in SPS
#define ADS126X_ADC2_RATE_100     0b01
#define ADS126X_ADC2_RATE_400     0b10
#define ADS126X_ADC2_RATE_800     0b11
#define ADS126X_ADC2_REF_INT      0b000 // reference inputs as positive and negative pairs
#define ADS126X_ADC2_REF_AIN0_AIN1 0b001
#define ADS126X_ADC2_REF_AIN2_AIN3 0b010
#define ADS126X_ADC2_REF_AIN4_AIN5 0b011
#define ADS126X_ADC2_REF_V        0b111
#define ADS126X_ADC2_GAIN_1       0b000 // gain of ADC2
#define ADS126X_ADC2_GAIN_2       0b001
#define ADS126X_ADC2_GAIN_4       0b010
#define ADS126X_ADC2_GAIN_8       0b011
#define ADS126X_ADC2_GAIN_16      0b100
#define ADS126X_ADC2_GAIN_32      0b101
#define ADS126X_ADC2_GAIN_64      0b110
#define ADS126X_ADC2_GAIN_128     0b111

/*!< Make a complete register map */
typedef union { // page 89
  struct {
    uint8_t REV_ID:5;         /*!< bit:  0.. 4 Revision ID                        */
    uint8_t DEV_ID:3;         /*!< bit:  5.. 8 Device ID                          */
  } bit;                      /*!< Structure used for bit access                  */
  uint8_t reg;                /*!< Type      used for register access             */
} ADS126X_ID_Type;

typedef union { // page 89
  struct {
    uint8_t INTREF:1;         /*!< bit:      0 Internal Reference Enable          */
    uint8_t VBIAS:1;          /*!< bit:      1 Level Shift Voltage Enable         */
    uint8_t :2;               /*!< bit:  2.. 3 Reserved                           */
    uint8_t RESET:1;          /*!< bit:      4 Reset Indicator                    */
    uint8_t :3;               /*!< bit:  5.. 7 Reserved                           */
  } bit;
  uint8_t reg;
} ADS126X_POWER_Type;

typedef union { // page 90
  struct {
    uint8_t CRC:2;            /*!< bit:  0.. 1 Checksum Byte Enable               */
    uint8_t STATUS:1;         /*!< bit:      2 Status Byte Enable                 */
    uint8_t TIMEOUT:1;        /*!< bit:      3 Serial Interface Time-Out Enable   */
    uint8_t :4;               /*!< bit:  4.. 7 Reserved                           */
  } bit;
  uint8_t reg;
} ADS126X_INTERFACE_Type;

typedef union { // page 91
  struct {
    uint8_t DELAY:4;          /*!< bit:  0.. 3 Conversion Delay                   */
    uint8_t CHOP:2;           /*!< bit:  4.. 5 Chop Mode Enable                   */
    uint8_t RUNMODE:1;        /*!< bit:      6 ADC Conversion Run Mode            */
    uint8_t REFREV:1;         /*!< bit:      7 Reference Mux Polarity Reversal    */
  } bit;
  uint8_t reg;
} ADS126X_MODE0_Type;

typedef union { // page 92
  struct {
    uint8_t SBMAG:3;          /*!< bit:  0.. 2 Sensor Bias Magnitude              */
    uint8_t SBPOL:1;          /*!< bit:      3 Sensor Bias Polarity               */
    uint8_t SBADC:1;          /*!< bit:      4 Sensor Bias ADC Connection         */
    uint8_t FILTER:3;         /*!< bit:  5.. 7 Digital Filter                     */
  } bit;
  uint8_t reg;
} ADS126X_MODE1_Type;

typedef union { // page 93
  struct {
    uint8_t DR:4;             /*!< bit:  0.. 3 Data Rate                          */
    uint8_t GAIN:3;           /*!< bit:  4.. 6 PGA Gain                           */
    uint8_t BYPASS:1;         /*!< bit:      7 PGA Bypass Mode                    */
  } bit;
  uint8_t reg;
} ADS126X_MODE2_Type;

typedef union { // page 94
  struct {
    uint8_t MUXN:4;           /*!< bit:  0.. 3 Negative Input Multiplexer         */
    uint8_t MUXP:4;           /*!< bit:  4.. 7 Positive Input Multiplexer         */
  } bit;
  uint8_t reg;
} ADS126X_INPMUX_Type;

typedef union { // page 95
  struct {
    uint8_t OFC:8;            /*!< bit:  0..7 Offset Calibration                  */
  } bit;
  uint8_t reg;
} ADS126X_OFCAL_Type;

typedef union { // page 95
  struct {
    uint8_t FSCAL:8;          /*!< bit:  0..7 Offset Calibration                  */
  } bit;
  uint8_t reg;
} ADS126X_FSCAL_Type;

// Some AVR definitions got in the way of this for me...
#ifndef __AVR__
typedef union { // page 96
  struct {
      uint8_t MUX1:4;         /*!< bit  0.. 3 IDAC1 Output Multiplexer            */
      uint8_t MUX2:4;         /*!< bit  4.. 7 IDAC2 Output Multiplexer            */
  } bit;
  uint8_t reg;
} ADS126X_IDACMUX_Type;
#else
typedef union { // page 96
  struct {
      uint8_t ADS_MUX1:4;         /*!< bit  0.. 3 IDAC1 Output Multiplexer            */
      uint8_t ADS_MUX2:4;         /*!< bit  4.. 7 IDAC2 Output Multiplexer            */
  } bit;
  uint8_t reg;
} ADS126X_IDACMUX_Type;
#endif

typedef union { // page 97
  struct {
    uint8_t MAG1:4;           /*!< bit 0.. 3 IDAC1 Current Magnitude              */
    uint8_t MAG2:4;           /*!< bit 4.. 7 IDAC2 Current Magnitude              */
  } bit;
  uint8_t reg;
} ADS126X_IDACMAG_Type;

typedef union { // page 98
  struct {
    uint8_t RMUXN:3;          /*!< bit 0.. 2 Reference Negative Input             */
    uint8_t RMUXP:3;          /*!< bit 3.. 5 Reference Positive Input             */
    uint8_t :2;               /*!< bit 6.. 7 Reserved                             */
  } bit;
  uint8_t reg;
} ADS126X_REFMUX_Type;

typedef union { // page 99
  struct {
    uint8_t MAGP:5;           /*!< bit 0.. 4 MAGP Output Magnitude                */
    uint8_t :2;               /*!< bit 5.. 6 Reserved                             */
    uint8_t OUTP:1;           /*!< bit     7 TDACP Output Connection              */
  } bit;
  uint8_t reg;
} ADS126X_TDACP_Type;

typedef union { // page 100
  struct {
    uint8_t MAGN:5;           /*!< bit 0.. 4 TDACN Output Magnitude               */
    uint8_t :2;               /*!< bit 5.. 6 Reserved                             */
    uint8_t OUTN:1;           /*!< bit     7 TDACN Output Connection              */
  } bit;
  uint8_t reg;
} ADS126X_TDACN_Type;

typedef union { // page 101
  struct {
    uint8_t CON0:1;           /*!< bit     0 GPIO[0] Pin Connection               */
    uint8_t CON1:1;           /*!< bit     1 GPIO[1] Pin Connection               */
    uint8_t CON2:1;           /*!< bit     2 GPIO[2] Pin Connection               */
    uint8_t CON3:1;           /*!< bit     3 GPIO[3] Pin Connection               */
    uint8_t CON4:1;           /*!< bit     4 GPIO[4] Pin Connection               */
    uint8_t CON5:1;           /*!< bit     5 GPIO[5] Pin Connection               */
    uint8_t CON6:1;           /*!< bit     6 GPIO[6] Pin Connection               */
    uint8_t CON7:1;           /*!< bit     7 GPIO[7] Pin Connection               */
  } bit;
  uint8_t reg;
} ADS126X_GPIOCON_Type;

typedef union { // page 102
  struct {
    uint8_t DIR0:1;           /*!< bit     0 GPIO[0] Pin Direction                */
    uint8_t DIR1:1;           /*!< bit     1 GPIO[1] Pin Direction                */
    uint8_t DIR2:1;           /*!< bit     2 GPIO[2] Pin Direction                */
    uint8_t DIR3:1;           /*!< bit     3 GPIO[3] Pin Direction                */
    uint8_t DIR4:1;           /*!< bit     4 GPIO[4] Pin Direction                */
    uint8_t DIR5:1;           /*!< bit     5 GPIO[5] Pin Direction                */
    uint8_t DIR6:1;           /*!< bit     6 GPIO[6] Pin Direction                */
    uint8_t DIR7:1;           /*!< bit     7 GPIO[7] Pin Direction                */
  } bit;
  uint8_t reg;
} ADS126X_GPIODIR_Type;

typedef union { // page 103
  struct {
    uint8_t DAT0:1;           /*!< bit     0 GPIO[0] Pin Data                     */
    uint8_t DAT1:1;           /*!< bit     1 GPIO[1] Pin Data                     */
    uint8_t DAT2:1;           /*!< bit     2 GPIO[2] Pin Data                     */
    uint8_t DAT3:1;           /*!< bit     3 GPIO[3] Pin Data                     */
    uint8_t DAT4:1;           /*!< bit     4 GPIO[4] Pin Data                     */
    uint8_t DAT5:1;           /*!< bit     5 GPIO[5] Pin Data                     */
    uint8_t DAT6:1;           /*!< bit     6 GPIO[6] Pin Data                     */
    uint8_t DAT7:1;           /*!< bit     7 GPIO[7] Pin Data                     */
  } bit;
  uint8_t reg;
} ADS126X_GPIODAT_Type;

typedef union { // page 104
  struct {
    uint8_t GAIN2:3;          /*!< bit 0.. 2 ADC2 Gain                            */
    uint8_t REF2:3;           /*!< bit 3.. 5 ADC2 Reference Input                 */
    uint8_t DR2:2;            /*!< bit 6.. 7 ADC2 Data Rate                       */
  } bit;
  uint8_t reg;
} ADS126X_ADC2CFG_Type;

typedef union { // page 105
  struct {
    uint8_t MUXN:4;           /*!< bit 0.. 3 ADC2 Negative Input Multiplexer      */
    uint8_t MUXP:4;           /*!< bit 4.. 7 ADC2 Positive Input Multiplexer      */
  } bit;
  uint8_t reg;
} ADS126X_ADC2MUX_Type;

typedef union { // page 106
  struct {
    uint8_t OFC2:8;           /*!< bit 0.. 7 ADC2 Offset Calibration              */
  } bit;
  uint8_t reg;
} ADS126X_ADC2OFC_Type;

typedef union { // page 106
  struct {
    uint8_t FSC2:8;           /*!< bit 0.. 7 ADC2 Offset Calibration              */
  } bit;
  uint8_t reg;
} ADS126X_ADC2FSC_Type;

typedef struct { // the entire register map, page 88
  __IO ADS126X_ID_Type        ID;         /**< \brief Offset: 0x00 (R/W  8) Device Identification          */
  __IO ADS126X_POWER_Type     POWER;      /**< \brief Offset: 0x01 (R/W  8) Power                          */
  __IO ADS126X_INTERFACE_Type INTERFACE;  /**< \brief Offset: 0x02 (R/W  8) Interface                      */
  __IO ADS126X_MODE0_Type     MODE0;      /**< \brief Offset: 0x03 (R/W  8) Mode0                          */
  __IO ADS126X_MODE1_Type     MODE1;      /**< \brief Offset: 0x04 (R/W  8) Mode1                          */
  __IO ADS126X_MODE2_Type     MODE2;      /**< \brief Offset: 0x05 (R/W  8) Mode2                          */
  __IO ADS126X_INPMUX_Type    INPMUX;     /**< \brief Offset: 0x06 (R/W  8) Input Multiplexer              */
  __IO ADS126X_OFCAL_Type     OFCAL0;     /**< \brief Offset: 0x07 (R/W  8) Offset Calibration 0           */
  __IO ADS126X_OFCAL_Type     OFCAL1;     /**< \brief Offset: 0x08 (R/W  8) Offset Calibration 1           */
  __IO ADS126X_OFCAL_Type     OFCAL2;     /**< \brief Offset: 0x09 (R/W  8) Offset Calibration 2           */
  __IO ADS126X_FSCAL_Type     FSCAL0;     /**< \brief Offset: 0x0A (R/W  8) Full-Scale Calibration 0       */
  __IO ADS126X_FSCAL_Type     FSCAL1;     /**< \brief Offset: 0x0B (R/W  8) Full-Scale Calibration 1       */
  __IO ADS126X_FSCAL_Type     FSCAL2;     /**< \brief Offset: 0x0C (R/W  8) Full-Scale Calibration 2       */
  __IO ADS126X_IDACMUX_Type   IDACMUX;    /**< \brief Offset: 0x0D (R/W  8) IDAC Multiplexer               */
  __IO ADS126X_IDACMAG_Type   IDACMAG;    /**< \brief Offset: 0x0E (R/W  8) IDAC Magnitude                 */
  __IO ADS126X_REFMUX_Type    REFMUX;     /**< \brief Offset: 0x0F (R/W  8) Reference Multiplexer          */
  __IO ADS126X_TDACP_Type     TDACP;      /**< \brief Offset: 0x10 (R/W  8) TDACP Output                   */
  __IO ADS126X_TDACN_Type     TDACN;      /**< \brief Offset: 0x11 (R/W  8) TDACN Negative Output          */
  __IO ADS126X_GPIOCON_Type   GPIOCON;    /**< \brief Offset: 0x12 (R/W  8) GPIO Connection                */
  __IO ADS126X_GPIODIR_Type   GPIODIR;    /**< \brief Offset: 0x13 (R/W  8) GPIO Direction                 */
  __IO ADS126X_GPIODAT_Type   GPIODAT;    /**< \brief Offset: 0x14 (R/W  8) GPIO Data                      */
  __IO ADS126X_ADC2CFG_Type   ADC2CFG;    /**< \brief Offset: 0x15 (R/W  8) ADC2 Configuration             */
  __IO ADS126X_ADC2MUX_Type   ADC2MUX;    /**< \brief Offset: 0x16 (R/W  8) ADC2 Input Multiplexer         */
  __IO ADS126X_ADC2OFC_Type   ADC2OFC0;   /**< \brief Offset: 0x17 (R/W  8) ADC2 Offset Calibration 0      */
  __IO ADS126X_ADC2OFC_Type   ADC2OFC1;   /**< \brief Offset: 0x18 (R/W  8) ADC2 Offset Calibration 1      */
  __IO ADS126X_ADC2FSC_Type   ADC2FSC0;   /**< \brief Offset: 0x19 (R/W  8) ADC2 Full-Scale Calibration 0  */
  __IO ADS126X_ADC2FSC_Type   ADC2FSC1;   /**< \brief Offset: 0x1A (R/W  8) ADC2 Full-Scale Calibration 1  */
} ADS126X_REGISTER_Type;

typedef union { // page 70
  struct {
    uint8_t RESET:1;          /*!< bit     0 Indicates device reset               */
    uint8_t PGAD_ALM:1;       /*!< bit     1 ADC1 PGA differential output alarm   */
    uint8_t PGAH_ALM:1;       /*!< bit     2 ADC1 PGA output high alarm           */
    uint8_t PGAL_ALM:1;       /*!< bit     3 ADC1 PGA output low alarm            */
    uint8_t REF_ALM:1;        /*!< bit     4 ADC1 low reference alarm             */
    uint8_t EXTCLK:1;         /*!< bit     5 Indicates ADC clock source           */
    uint8_t ADC1:1;           /*!< bit     6 status of ADC1 conversion data       */
    uint8_t ADC2:1;           /*!< bit     7 status of ADC2 conversion data       */
  } bit;
  uint8_t reg;
} ADS126X_STATUS_Type;

#ifdef __cplusplus
}
#endif

#endif // define ADS126X_DEFINITIONS_H
