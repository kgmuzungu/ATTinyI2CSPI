# ATTinyI2CSPI
Thermometer, ATTiny85 MAX31855 with thermocoupler, LCD HD44780 display with PFC8574, Arduino IDE 1.6.6

The challenge is to get the ATTiny talk I2C to the PFC8574 port expander that is connected to the LCD display and SPI to the MAX31855. Plenty of code is out there let's just put it together. 

General: the MAX31855 (from Adafruit: https://www.adafruit.com/product/269, https://www.maximintegrated.com/en/products/analog/sensors-and-sensor-interface/MAX31855.html) is an amplifier for also K-type thermocouplers (K-type = defines the two metals used to sense temp). About how to measure temperature you find in Maxim Application notes. K-type thermocouplers are non-linear and are not compensated by the MAX31855. But for temperature ranges from -50 to 300C you can live without compensation. You can figure out the accuracy of this implementation yourself (depends also on the thermocoupler you used). You could do some compensation using the ATTiny (but you might have to optimize the used libraries to free up some memory)

Components used:
- ATTiny85
- MAX31855
- LCD HD44780 display with PFC8574
- Arduino Uno for programming the ATTiny

Great thanks for the fantastic libraries I could use:
- by F Malpartida:  NewLiquidCrystal https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home
- optional: by J Christensen: TinySPI https://github.com/JChristensen/tinySPI
- by Limor Fried/Ladyada: Adafruit_MAX31855 (modified) https://github.com/adafruit/Adafruit-MAX31855-library

Wiring: the pin numbers outside the box are the IDE pin numbers. Inside the box are the physical pin numbers. 
```			  ATTiny85
                         +-----+
                RESET -5-|1   8|--- Vcc
..to	      { SCL   -3-|2   7|-2- CLK = CLOCK_PIN }
PCF8574	      { SDA   -4-|3   6|-1- CS	= LATCH_PIN	} ..to Adafruit MAX31855
                GND   ---|4   5|-0- DO	= DATA_PIN  }
                         +-----+
```			   


Setting up the Arduino IDE:
Open the sketch ArduinoISP in examples and upload it to the Arduino Uno. Connect the ATTiny85 as in
http://highlowtech.org/?p=1695
http://highlowtech.org/?p=1706

Then go to
tools --> board	-->ATTiny

tools --> processor-->ATTiny85

tools --> clock	-->8MHz(internal)

tools --> programmer-->Arduino as ISP

If you have a vergin ATTiny85 you have to burn the bootloader first.

Get the display working using NewLiquidCrystal:
The ATTiny has no I2C hardware. Therefore we will use LiquidCrystal_SI2C.h, the SoftwareI2C. Copy the unpacked NewLiquidCrystal into %USER%\Documents\Arduino\libraries\NewliquidCrystal. But there is a catch. To get it all compiled you have to delete LiquidCrystal_I2C* and some other files, because all C files in the ...\libraries\ folder will be compiled. With the Arduino IDE set to board --> ATTiny some values are passed to the compiler that select the right constants/register settings in Arduino.h If you leave the LiquidCrystal_I2C in the ...\libraries\ folder you will get an error because it tries to incorporate Wire.h . ATTiny doesnt have the registers used by that library. 

Get the SPI communication to the MAX31855 working:
The ATTiny has no SPI hardware either but it has an USI. In Adafruit_MAX31855.cpp include tinySPI.h instead of SPI.h or you delete all hardwareSPI related code. I actually only used the softwareSPI in Adafruit_MAX31855 and that worked like a treat. 

To get the I2C address of your expander use https://github.com/MartyMacGyver/Arduino_I2C_Scanner/blob/master/I2C_Scanner/I2C_Scanner.ino
