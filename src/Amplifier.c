#include "Shared.h"
#include "Amplifier.h"

uint AmplifierInitPins(void)
{
    uint sliceNumber = 0;
    uint channelNumber = 0;

    gpio_set_function(GPIO_PIN_AMPLIFIER_PWM, GPIO_FUNC_PWM);

    sliceNumber = pwm_gpio_to_slice_num(GPIO_PIN_AMPLIFIER_PWM);

    channelNumber = pwm_gpio_to_channel(GPIO_PIN_AMPLIFIER_PWM);

    AmplifierSetSliceFrequencyAndDutyCycle(sliceNumber, channelNumber, 2000, 50);

    return sliceNumber;
}


uint32_t AmplifierSetSliceFrequencyAndDutyCycle(uint sliceNumber, uint chan, uint32_t frequency, int dutyCycle)
{
    uint32_t clock = 125000000;

    uint32_t divider16 = clock / frequency / 4096 +
                         (clock % (frequency * 4096) != 0);

    if (divider16 / 16 == 0)
    {
        divider16 = 16;
    }

    uint32_t wrap = clock * 16 / divider16 / frequency - 1;

    pwm_set_clkdiv_int_frac(
        sliceNumber,
        divider16 / 16,
        divider16 & 0xF);

    pwm_set_wrap(sliceNumber, wrap);

    pwm_set_chan_level(sliceNumber, chan, wrap * dutyCycle / 100);

    return wrap;
}



void AmplifierBeepSpeaker(uint amplifierPwmPinSliceNumber, uint timeMs, bool flashLed)
{
    if(flashLed)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    }
    
    gpio_set_function(GPIO_PIN_AMPLIFIER_PWM, GPIO_FUNC_PWM);
    pwm_set_enabled(amplifierPwmPinSliceNumber, true);

    sleep_ms(timeMs);

    if(flashLed)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    }

    pwm_set_enabled(amplifierPwmPinSliceNumber, false);

    // We completely disable PWM to reduce noise when the speaker isnt supposed to beep
    gpio_set_function(GPIO_PIN_AMPLIFIER_PWM, GPIO_FUNC_SIO);
    gpio_set_dir(GPIO_PIN_AMPLIFIER_PWM, GPIO_OUT);
    gpio_put(GPIO_PIN_AMPLIFIER_PWM, 0);
}
