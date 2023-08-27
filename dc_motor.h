#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#define GPIO_PIN_TO_BE_ENABLED 12
#define SLICE_NUM_INIT 0
#define CHANNEL_INIT 0
static uint8_t GPIO_DC = GPIO_PIN_TO_BE_ENABLED;
static uint8_t SLICE_NUM = SLICE_NUM_INIT;
static uint8_t CHANNEL = CHANNEL_INIT;
static uint8_t DUTY = 10;

void initialize_pwm_channel(uint8_t g_pin)
{
    gpio_set_function(g_pin, GPIO_FUNC_PWM);
    SLICE_NUM = pwm_gpio_to_slice_num(g_pin);
    CHANNEL = pwm_gpio_to_channel(g_pin);
}

uint32_t set_frequency(uint32_t frequency)
{
    uint32_t clock = clock_get_hz(clk_sys);
    uint32_t div16 = clock / frequency / 4096 + (clock % (frequency * 4096) != 0);
    if (div16 / 16 == 0)
    {
        div16 = 16;
    }
    uint32_t wrap = clock * 16 / div16 / frequency - 1;

    pwm_set_clkdiv_int_frac(SLICE_NUM, div16 / 16, div16 & 0xF);
    pwm_set_wrap(SLICE_NUM, wrap);

    return wrap;
}

void set_duty(uint32_t d, uint32_t w)
{
    pwm_set_chan_level(SLICE_NUM, CHANNEL, w * d / 100);
    pwm_set_enabled(SLICE_NUM, true);
}