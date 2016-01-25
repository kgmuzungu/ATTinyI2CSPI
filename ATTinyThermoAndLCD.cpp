#include <LiquidCrystal_SI2C.h>
#include "Adafruit_MAX31855.h"

#define HARDWARE_SPI 1            //set to 1 to use hardware SPI, set to 0 to use shiftOut (software SPI)

//pin definitions
const int8_t LATCH_PIN = 0;           //storage register clock (slave select)
const int8_t DATA_PIN  = 1;           //data in
const int8_t CLOCK_PIN = 2;           //shift register clock
String tempStr,chipTempStr;

// what PINs are set in ...\NewLiquidCrystal\SI2CIO.cpp 
// class instantiated but not initialised
LiquidCrystal_SI2C  lcd(0x3F,2,1,0,4,5,6,7, 3, POSITIVE);

//would use a hardwareSPI... using some USI stuff?
//Adafruit_MAX31855 thermocouple(LATCH_PIN);
//uses a software SPI solution
//Adafruit_MAX31855(int8_t SCLK, int8_t CS, int8_t MISO)
Adafruit_MAX31855 thermocouple(CLOCK_PIN, LATCH_PIN, DATA_PIN);

void setup() {

  lcd.begin (16,2);
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  delay(10);

  lcd.home();                   // set cursor to home
  lcd.print("## TempMeter ##"); 
  lcd.setCursor ( 0, 1 );
  lcd.print("V1.0");
  
  // wait for MAX chip to stabilize
  // wait before doing anything on the SPI pins that are used to program the ATTiny!!
  delay(3000);

  lcd.home();                   // set cursor to home
  lcd.print("                "); 
  lcd.setCursor ( 0, 1 );
  lcd.print("                "); 

  //pullups on for unused pins to minimize power consumption
  //pinMode(3, INPUT_PULLUP);
  //pinMode(4, INPUT_PULLUP);
}

void loop() {
  
  double temp = thermocouple.readCelsius();
  tempStr = "T     = " + String(temp,2);
  double chipTemp = thermocouple.readInternal();
  chipTempStr = "Tchip = " + String(chipTemp,2);
  
  lcd.setCursor ( 0, 0 );
  lcd.print(tempStr);
  lcd.setCursor ( 0, 1 );
  lcd.print(chipTempStr);
  delay(500);
}
