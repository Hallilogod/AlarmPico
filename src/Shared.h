#pragma once

/*
---- CONFIGURATION ----
*/

// Whether the expected distance should be re-measured after UPDATE_EXPECTED_DISTANCE_TIME_SECONDS of critical detection.
#define UPDATE_EXPECTED_DISTANCE 0

/* Time of a critical distance detection after which a re-measurement of the expected distance should take place,
    will be rounded up to the next multiple of ALARM_LENGTH_SECONDS.
*/
#define UPDATE_EXPECTED_DISTANCE_TIME_SECONDS 10

// Time to wait between distance measurements when done repeatedly, should be over 60 according to HC-SR04 datasheet.
#define TIME_BETWEEN_DISTANCE_MEASUREMENTS_MS 80

// Time to wait before the program initializes.
#define INIT_WAIT_TIME_SECONDS 6

// The distance difference in cm at which an alarm should trigger.
#define CRITICAL_DISTANCE_DIFFERENCE_CM 3

// Time, in seconds, how long an alarm should beep (consisting of single beeps). 
#define ALARM_LENGTH_SECONDS 3

// Duration of a single beep, should not be bigger than ALARM_LENGTH_SECONDS * 1000.
#define SINGLE_BEEP_DURATION_MS 250

// Frequency, in Hertz, of the alarm beeps
#define BEEP_FREQUENCY_HZ 2000

/*
---- END CONFIGURATION ----
*/
