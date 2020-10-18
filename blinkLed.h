#ifndef _BLINKLED
#define _BLINKLED

#include <Arduino.h>

class BlinkLed
{
  public:
    bool update(int co2Value, int tvocValue);
    int getLedState() { return m_ledState; }
    int getInterval() { return m_interval; }
    
  private:
    void updateInterval(int co2Value, int tvocValue);
    bool updateLedState();

    // ledState used to set the LED
    int m_ledState = LOW;

    // blinking interval in milliseconds
    int m_interval = -1;

    // will store last time LED was updated
    unsigned long m_previousMillis = 0;
};
#endif
