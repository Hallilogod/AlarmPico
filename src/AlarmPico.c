#include "AlarmPico.h"


void InfoBlinkLed(uint count)
{
    while (count--)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(300);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(300);
    }
}

int main(void)
{
    stdio_init_all();

    sleep_ms(1000 * INIT_WAIT_TIME_SECONDS);

    printf("Hello World from RP2350!\n");
    printf("Pico Alarm project built at %s\n" __TIME__);

    // Initialise the Wi-Fi chip so we can blink the LED
    if (cyw43_arch_init())
    {
        printf("Wi-Fi init failed\n");
        return 1;
    }

    SensorInitGpioPins();

    // Initialize pwm pin for sending the analog audio signal to the amp
    uint amplifierPwmPinSliceNumber = AmplifierInitPins();

    AmplifierSetSliceFrequencyAndDutyCycle(
        amplifierPwmPinSliceNumber,
        pwm_gpio_to_channel(GPIO_PIN_AMPLIFIER_PWM),
        BEEP_FREQUENCY_HZ,
        50);

    FLOAT expectedDistanceCm = {0};

    SensorMeasureAverageDistance(1, &expectedDistanceCm);

    printf("Expected average distance: %u.%ucm\n", expectedDistanceCm.integerPart, expectedDistanceCm.fractionalPart);

    InfoBlinkLed(2);

#if UPDATE_EXPECTED_DISTANCE
    uint criticalDetectionDurationSeconds = 0;
#endif

    while (1)
    {
        FLOAT currentDistanceCm = {0};
        SensorMeasureDistance(&currentDistanceCm);

#if UPDATE_EXPECTED_DISTANCE
        if (criticalDetectionDurationSeconds >= UPDATE_EXPECTED_DISTANCE_TIME_SECONDS)
        {
            printf("Re-measuring expected distance...\n");

            sleep_ms(TIME_BETWEEN_DISTANCE_MEASUREMENTS_MS);

            SensorMeasureAverageDistance(1, &expectedDistanceCm);

            printf("New expected distance: %u.%ucm\n", expectedDistanceCm.integerPart, expectedDistanceCm.fractionalPart);

            criticalDetectionDurationSeconds = 0;
        }

#endif

        printf("Current Distance: %u.%ucm\n", currentDistanceCm.integerPart, currentDistanceCm.fractionalPart);

        if (!SensorDetectCriticalDistanceChange(&expectedDistanceCm, &currentDistanceCm))
        {
#if UPDATE_EXPECTED_DISTANCE
            criticalDetectionDurationSeconds = 0;
#endif
            sleep_ms(TIME_BETWEEN_DISTANCE_MEASUREMENTS_MS);

            continue;
        }

        for (int i = 0; i < ALARM_LENGTH_SECONDS * 1000; i += (SINGLE_BEEP_DURATION_MS * 2))
        {
            AmplifierBeepSpeaker(amplifierPwmPinSliceNumber, SINGLE_BEEP_DURATION_MS, true);
            sleep_ms(SINGLE_BEEP_DURATION_MS);
        }
#if UPDATE_EXPECTED_DISTANCE
        criticalDetectionDurationSeconds += ALARM_LENGTH_SECONDS;
#endif
    }

    return 0;
}
