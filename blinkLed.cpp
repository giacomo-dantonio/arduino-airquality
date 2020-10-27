#include "constants.h"
#include "blinkLed.h"

// compute the interval at which to blink (in milliseconds)
// depending on the CO2 and tVOC values.
int getIntervalForValue(int value, int uncritical, int critical)
{
    if (value < uncritical)
    {
        return -1;
    }
    else if (value < critical)
    {
        double parameter = (double)(value - uncritical)
            / (critical - uncritical);
        return (int)(
            biggestInterval - parameter
            * (biggestInterval - smallestInterval));
    }
    else
    {
        return smallestInterval;
    }
}

void BlinkLed::updateInterval(int co2Value, int tvocValue)
{
    int co2Interval = getIntervalForValue(
        co2Value, uncriticalCO2value, criticalCO2value);
    int tvocInterval = getIntervalForValue(
        tvocValue, uncriticalTVOCvalue, criticalTVOCvalue);

    if (co2Interval < 0)
    {
        m_interval = tvocInterval;
    }
    else if (tvocInterval < 0)
    {
        m_interval = co2Interval;
    }
    else
    {
        m_interval = min(co2Interval, tvocInterval);
    }
}

bool BlinkLed::updateLedState()
{
    // check to see if it's time to blink the LED; that is, if the difference
    // between the current time and last time you blinked the LED
    // is bigger than the interval at which you want to blink the LED.
    unsigned long currentMillis = millis();
    if (currentMillis < m_previousMillis)
    {
        // integer overflow
        m_previousMillis = 0;
    }

    if (m_interval < 0 && m_ledState == HIGH)
    {
        m_ledState = LOW;
        return true;
    }
    else if (currentMillis - m_previousMillis >= m_interval)
    {
        // save the last time you blinked the LED
        m_previousMillis = currentMillis;

        // if the LED is off turn it on and vice-versa:
        m_ledState = (m_ledState == LOW) ? HIGH : LOW;

        return true;
    }

    return false;
}

bool BlinkLed::update(int co2Value, int tvocValue)
{
    updateInterval(co2Value, tvocValue);
    return updateLedState();
}
