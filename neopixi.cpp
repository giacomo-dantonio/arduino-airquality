#include <Arduino.h>

#include "constants.h"
#include "neopixi.h"

// compute a parameter value for the neopixel display
// depending on the CO2 and tVOC values.
double getParameterForValue(int value, int uncritical, int critical)
{
    if (value < uncritical)
    {
        return 0;
    }
    else if (value <  2 * critical)
    {
        double parameter = (double)(value - uncritical)
            / (2 * critical - uncritical);
        return parameter;
    }
    else
    {
        return 1;
    }
    
}

// compute a parameter value for the color of the neopixel display
// 0 is green and 1 is red
double getSeverityForValue(int value, int uncritical, int critical)
{
    if (value < critical)
    {
        return 0;
    }
    else if (value <  2 * critical)
    {
        return (double)(value - critical) / critical;
    }
    else
    {
        return 1;
    }
}

bool NeoPixi::update(int co2Value, int tvocValue)
{
    double parameter = max(
        getParameterForValue(co2Value, uncriticalCO2value, criticalCO2value),
        getParameterForValue(tvocValue, uncriticalTVOCvalue, criticalTVOCvalue));
    
    m_activeLed = parameter == 0 ? -1 : (int)floor(parameter * 12);
    m_activeLedIntensity = parameter * 12 - m_activeLed;

    m_severity = max(
        getSeverityForValue(co2Value, uncriticalCO2value, criticalCO2value),
        getSeverityForValue(tvocValue, uncriticalTVOCvalue, criticalTVOCvalue));

    return true;
}

void NeoPixi::getLedHSVColor(int index, int & hue, int & sat, int & val)
{
    if (index <= m_activeLed)
    {
        // 0 is green, 22027 is red
        hue = (int)floor(22027 * m_severity);
        val = index < m_activeLed
            ? m_value
            : (int)floor(m_activeLedIntensity * m_value);
        sat = m_saturation;
    }
    else
    {
        hue = 0;
        sat = 0;
        val = 0;
    }
    
}
