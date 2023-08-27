
#include "ultra_servo.h"

int main()
{
    //---------------------------
    uint wrap_7, d = DUTY_7;
    initialize_pwm_channel_7(GPIO_PIN_7);
    //---------------------------
    const uint trig_pin = 11;
    const uint echo_pin = 10;

    uint trig, echo;
    uint dist;

    gpio_init(trig_pin);
    gpio_set_dir(trig_pin, GPIO_OUT);
    gpio_init(echo_pin);
    gpio_set_dir(echo_pin, GPIO_IN);

    stdio_init_all();
    //---------------------------
    wrap_7 = set_frequency_7(FREQ_7);
    set_duty_7(DUTY_7, wrap_7);
    pwm_set_enabled(SLICE_NUM_7, false);
    sleep_ms(1000);
    //---------------------------
    while (1)
    {
        gpio_put(trig_pin, 0);
        sleep_ms(100);
        gpio_put(trig_pin, 1);
        sleep_us(2);
        gpio_put(trig_pin, 0);

        while (gpio_get(echo_pin) == 0)
            trig = (uint)time_us_64();

        while (gpio_get(echo_pin) == 1)
            echo = (uint)time_us_64();

        dist = (echo - trig) * 17165.0 / 1000000;
        //-----------servo----------------
        thirdfnc(dist, d, wrap_7);
        //--------------------------------
        sleep_ms(500);
    }
    return 0;
}
