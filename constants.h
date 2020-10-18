#ifndef _CONSTANTS
#define _CONSTANTS

// blinking interval at uncriticalCO2value
const int biggestInterval = 5000;

// blinking interval at criticalCO2value
const int smallestInterval = 100;

// critical values
// CO2 values are in ppm (particles per million)
const int uncriticalCO2value = 800;
const int criticalCO2value = 1000;
// tVOC values are in ppb (particles per billion)
const int uncriticalTVOCvalue = 440;
const int criticalTVOCvalue = 1300; 
#endif
