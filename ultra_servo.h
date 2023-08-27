
#include <stdio.h>
#include <time.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"

#define GPIO_PIN_TO_BE_ENABLED_7 7
#define SLICE_NUM_INIT_7 0
#define CHANNEL_INIT_7 0
#define FREQ_7 50 // Center=1.5ms Left=0.5ms Right=2.5ms

void initialize_pwm_channel_7(uint8_t g_pin);
uint32_t set_frequency_7(uint32_t frequency);
void set_duty_7(uint32_t d, uint32_t w);

static uint8_t GPIO_PIN_7 = GPIO_PIN_TO_BE_ENABLED_7;
static uint8_t SLICE_NUM_7 = SLICE_NUM_INIT_7;
static uint8_t CHANNEL_7 = CHANNEL_INIT_7;
static uint8_t DUTY_7 = FREQ_7 * 0.15;
static uint8_t Duty_Max_7 = FREQ_7 * 0.25;
static uint8_t Duty_Min_7 = FREQ_7 * 0.05 + 1;

void initialize_pwm_channel_7(uint8_t g_pin)
{
    gpio_set_function(g_pin, GPIO_FUNC_PWM);
    SLICE_NUM_7 = pwm_gpio_to_slice_num(g_pin);
    CHANNEL_7 = pwm_gpio_to_channel(g_pin);
}

uint32_t set_frequency_7(uint32_t frequency_7)
{
    uint32_t clock_7 = clock_get_hz(clk_sys);
    uint32_t div16_7 = clock_7 / frequency_7 / 4096 + (clock_7 % (frequency_7 * 4096) != 0);
    if (div16_7 / 16 == 0)
    {
        div16_7 = 16;
    }
    uint32_t wrap_7 = clock_7 * 16 / div16_7 / frequency_7 - 1;

    pwm_set_clkdiv_int_frac(SLICE_NUM_7, div16_7 / 16, div16_7 & 0xF);
    pwm_set_wrap(SLICE_NUM_7, wrap_7);

    return wrap_7;
}

void set_duty_7(uint32_t d, uint32_t w)
{
    pwm_set_chan_level(SLICE_NUM_7, CHANNEL_7, w * d / 100);
    pwm_set_enabled(SLICE_NUM_7, true);
}
void thirdfnc(uint dist, uint d, uint wrap_7)
{
    static int a;
    if (dist <= 8)
    {
        a = (a + 1) % 2;
    }
    switch (a)
    {
    case 0:
        if (dist <= 8)
        {
            d = Duty_Max_7;
            set_duty_7(d, wrap_7);
            printf("case0\n");
            break;
        }
    case 1:
        if (dist <= 8)
        {
            d = Duty_Min_7;
            set_duty_7(d, wrap_7);
            printf("case1\n");
            break;
        }
    default:
        break;
    }
}