#include "Shared.h"
#include "Sensor.h"


void SensorInitGpioPins(void)
{
    gpio_init(GPIO_PIN_HCSR04_TRIGGER);
    gpio_init(GPIO_PIN_HCSR04_ECHO);

    // gpioPinTrigger is where we send a short HIGH to the HC-SR04
    gpio_set_dir(GPIO_PIN_HCSR04_TRIGGER, GPIO_OUT);
    gpio_set_dir(GPIO_PIN_HCSR04_ECHO, GPIO_IN);

    gpio_put(GPIO_PIN_HCSR04_TRIGGER, 0);
}


void SensorMeasureDistance(PFLOAT pDistanceInCm)
{
    // Inform ultrasonic sensor
    gpio_put(GPIO_PIN_HCSR04_TRIGGER, 0);

    sleep_us(2);

    gpio_put(GPIO_PIN_HCSR04_TRIGGER, 1);

    // HC-SR04 needs a HIGH for atleast 10us
    sleep_us(10);

    gpio_put(GPIO_PIN_HCSR04_TRIGGER, 0);

    // Im scared this shit compiler will remove our empty loops without a volatile var
    volatile bool pinStatus = 0;

    // Wait for echo pin to go HIGH, this is where the sensor starts emitting the ultrasonic wave
    while (!(pinStatus = gpio_get(GPIO_PIN_HCSR04_ECHO)))
    {

    }

    // When echo pin is HIGH, capture the current time because we need it for the calculation of the distance
    uint64_t startSensorTime = time_us_64();


    while (pinStatus = gpio_get(GPIO_PIN_HCSR04_ECHO))
    {

    }

    uint64_t stopSensorTime = time_us_64();


    uint64_t measurementTime = stopSensorTime - startSensorTime;


    int distanceInCmTimes100 = (measurementTime * 343) / (100 * 2);

    uint distanceInCmFloor = distanceInCmTimes100 / 100;

    uint distanceInCmRemainder = distanceInCmTimes100 - ((int)(distanceInCmFloor) * 100);

    
    pDistanceInCm->integerPart = distanceInCmFloor;
    pDistanceInCm->fractionalPart = distanceInCmRemainder;

}


void SensorMeasureAverageDistance(uint measurementTimeSeconds, PFLOAT pAverageDistance)
{
    uint loopIterations = ((measurementTimeSeconds * 1000) / TIME_BETWEEN_DISTANCE_MEASUREMENTS_MS) + 1;

    for (int i = 0; i < loopIterations; i++)
    {
        FLOAT currentDistanceCm = {0};
        SensorMeasureDistance(&currentDistanceCm);

        pAverageDistance->integerPart += currentDistanceCm.integerPart;
        pAverageDistance->fractionalPart += currentDistanceCm.fractionalPart;

        sleep_ms(TIME_BETWEEN_DISTANCE_MEASUREMENTS_MS);
    }

    pAverageDistance->integerPart /= loopIterations;
    pAverageDistance->fractionalPart /= loopIterations;

}


bool SensorDetectCriticalDistanceChange(PFLOAT pExpectedDistanceCm, PFLOAT pMeasuredDistanceCm)
{
    uint expectedDistanceScaled = (pExpectedDistanceCm->integerPart * 100 + pExpectedDistanceCm->fractionalPart);
    uint measuredDistanceScaled = (pMeasuredDistanceCm->integerPart * 100 + pMeasuredDistanceCm->fractionalPart);

    uint distanceChange = (expectedDistanceScaled > measuredDistanceScaled) ? (expectedDistanceScaled - measuredDistanceScaled) : (measuredDistanceScaled - expectedDistanceScaled);

    return distanceChange > (CRITICAL_DISTANCE_DIFFERENCE_CM * 100);
}
