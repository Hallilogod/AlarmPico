#pragma once

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/pwm.h"


#define GPIO_PIN_AMPLIFIER_PWM 16


/*
Initializes the PWM pin for the Amplifier for the alarm speaker and the GPIO pin for the transistor switch.

@param none

@return The slice number of the amplifier's PWM pin
*/
uint AmplifierInitPins(void);

/*
Sets the frequency and duty cycle for a slice.

@param sliceNumber Number of the target slice
@param chan Channel of target slice
@param frequency Frequency in Hertz for the target slice
@param dutyCycle Duty cycle for the target slice

@return The slice's wrap value
*/
uint32_t AmplifierSetSliceFrequencyAndDutyCycle(uint sliceNumber, uint chan, uint32_t frequency, int dutyCycle);


/*
Beeps the speaker connected to the amplifier

@param amplifierPwmPinSliceNumber Slice number of the amplifier's PWM pin
@param timeMs Time to beep in milliseconds
@param flashLed Whether to turn on the onboard LED while beeping the speaker

@return none
*/
void AmplifierBeepSpeaker(uint amplifierPwmPinSliceNumber, uint timeMs, bool flashLed);
