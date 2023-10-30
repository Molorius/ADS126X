#include <ADS126X.h>

ADS126X adc; // start the class

int chip_select = 5; // Arduino pin connected to CS on ADS126X

int pos_pin = 0; // ADS126X pin AIN0, for positive input
//int pos_pin = ADS126X_AIN0;

int neg_pin = 1; // ADS126X pin AIN1, for negative input
//int neg_pin = ADS126X_AIN1;


void setup() {
  Serial.begin(115200);
  
  adc.begin(chip_select); // setup with chip select pin
  adc.startADC1(pos_pin, neg_pin); // start conversion on ADC1
  
  Serial.println("Reading Voltages:");
}

void loop() {
  long voltage = adc.readADC1(); // read the voltage
  Serial.println(voltage); // send voltage through serial
  delay(1000); // wait 1 second
}
