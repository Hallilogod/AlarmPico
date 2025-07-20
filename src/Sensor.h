#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"

#define GPIO_PIN_HCSR04_ECHO 14
#define GPIO_PIN_HCSR04_TRIGGER 15

typedef struct _FLOAT
{
    uint integerPart;
    // Two-digit fractional part
    uint fractionalPart;
} FLOAT, *PFLOAT;

/*
Initializes the GPIO pins for the HC-SR04 ultrasonic sensor.
@param none
@return none
*/
void SensorInitGpioPins(void);

/*
Measures the current distance via the sensor.

@param pDistanceInCm Pointer to a FLOAT to recieve the measured distance in cm

@return none
*/
void SensorMeasureDistance(PFLOAT pDistanceInCm);



/*
Measures the average distance over a specified time.

@param measurementTimeSeconds The time, in seconds, of the measurement process which will measure every TIME_BETWEEN_DISTANCE_MEASUREMENTS_MS milliseconds.
@param pAverageDistance A pointer to a FLOAT that recieves the average distance measured.

@return none
*/
void SensorMeasureAverageDistance(uint measurementTimeSeconds, PFLOAT pAverageDistance);


/*
Detects whether a measured distance is critical in relation to the expected distance based on CRITICAL_DISTANCE_DIFFERENCE_CM.

@param pExpectedDistanceCm The expected measurement distance, in cm
@param pMeasuredDistanceCm The actual distance that was measured 

@return true if pMeasuredDistanceCm is critical in relation to pExpectedDistanceCm, false otherwise

*/
bool SensorDetectCriticalDistanceChange(PFLOAT pExpectedDistanceCm, PFLOAT pMeasuredDistanceCm);
