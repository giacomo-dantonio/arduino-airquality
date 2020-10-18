/******************************************************************************
  Read basic CO2 and TVOCs

  Marshall Taylor @ SparkFun Electronics
  Nathan Seidle @ SparkFun Electronics

  April 4, 2017

  https://github.com/sparkfun/CCS811_Air_Quality_Breakout
  https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library

  Read the TVOC and CO2 values from the SparkFun CSS811 breakout board

  A new sensor requires at 48-burn in. Once burned in a sensor requires
  20 minutes of run in before readings are considered good.

  Hardware Connections (Breakoutboard to Arduino):
  3.3V to 3.3V pin
  GND to GND pin
  SDA to A4
  SCL to A5

******************************************************************************/
#include <Wire.h>

//Click here to get the library: http://librarymanager/All#SparkFun_CCS811
#include "SparkFunCCS811.h"

#include "constants.h"
#include "blinkLed.h"

//Default I2C Address
#define CCS811_ADDR 0x5B
//Alternate I2C Address
//#define CCS811_ADDR 0x5A

CCS811 mySensor(CCS811_ADDR);
BlinkLed blinkLed;

// the number of the LED pin
const int ledPin =  2;

int lastCO2value = 0;
int lastTVOCvalue = 0;

void setup()
{
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
  Serial.println("CCS811 Air quality Ampel");

  // Initialize I2C Hardware
  Wire.begin();

  if (mySensor.begin() == false)
  {
    Serial.print("CCS811 error. Please check wiring. Freezing...");
    while (1)
      ;
  }
}

void updateLed()
{
  if (blinkLed.update(lastCO2value, lastTVOCvalue))
  {
      // set the LED with the ledState of the variable:
      digitalWrite(ledPin, blinkLed.getLedState());
  }
}

bool readAirQuality()
{
  // Check to see if data is ready with .dataAvailable()
  if (mySensor.dataAvailable())
  {
    // If so, have the sensor read and calculate the results.
    mySensor.readAlgorithmResults();

    // Returns calculated CO2 reading
    lastCO2value = mySensor.getCO2();

    // Returns calculated TVOC reading
    lastTVOCvalue = mySensor.getTVOC();

    return true;
  }
  return false;
}

void loop()
{
  bool dataAvailable = readAirQuality();
  updateLed();

  if (dataAvailable)
  {
    Serial.print("CO2[");
    Serial.print(lastCO2value);
    Serial.print("] tVOC[");
    Serial.print(lastTVOCvalue);
    // Display the time since program start
    // Serial.print("] millis[");
    // Serial.print(millis());
    Serial.print("] interval[");
    Serial.print(blinkLed.getInterval());
    Serial.print("]");
    Serial.println();
  }
  
  delay(10); //Don't spam the I2C bus
}
