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

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Wire.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Click here to get the library: http://librarymanager/All#SparkFun_CCS811
#include "SparkFunCCS811.h"

#include "constants.h"
#include "blinkLed.h"
#include "neopixi.h"

// Default I2C Address
#define CCS811_ADDR 0x5B
// Alternate I2C Address
// #define CCS811_ADDR 0x5A

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 12

CCS811 mySensor(CCS811_ADDR);
BlinkLed blinkLed;
NeoPixi neopixi(LED_COUNT);

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// the number of the LED pin
const int ledPin = 3;

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

  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(25); // Set BRIGHTNESS to about 1/10 (max = 255)
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

void updateNeopixi()
{
    if (neopixi.update(lastCO2value, lastTVOCvalue))
    {
        strip.clear();
        for (int i = 0; i <= min(neopixi.getActivePixel(), LED_COUNT - 1); i++)
        {
            int hue, saturation, value;
            neopixi.getLedHSVColor(i, hue, saturation, value);
            uint32_t color = strip.ColorHSV(hue, saturation, value);
            strip.setPixelColor(i, color);
        }
        strip.show();
    }
}

void loop()
{
    bool dataAvailable = readAirQuality();
    updateLed();
    updateNeopixi();

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
        Serial.print("] severity[");
        Serial.print(neopixi.getSeverity());
        Serial.print("]");

        Serial.println();
    }

    delay(10); //Don't spam the I2C bus
}
