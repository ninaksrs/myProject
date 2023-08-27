/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "ssd1306_font.h"
#include "nec_receive.h"
#include "stepper_motor.h"
#include "OLED.h"
#include "basic_divices_gpio.h"
#include "dc_motor.h"
#include "buzzer_song.h"
#include "ultra_servo.h"

#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"

void init_HW()
{
    // I2C is "open drain", pull ups to keep signal high when no data is being
    // sent
    i2c_init(i2c_default, SSD1306_I2C_CLK * 1000);
    gpio_set_function(PICO_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_I2C_SDA_PIN);
    gpio_pull_up(PICO_I2C_SCL_PIN);

    // run through the complete initialization process
    SSD1306_init();
}

int main()
{
    //---------ULTRA_SERVO------------
    uint wrap_7, d = DUTY_7;
    initialize_pwm_channel_7(GPIO_PIN_7);
    //---------------------------
    const uint trig_pin = 11;
    const uint echo_pin = 10;

    uint trig, echo;
    uint dist = 1;

    gpio_init(trig_pin);
    gpio_set_dir(trig_pin, GPIO_OUT);
    gpio_init(echo_pin);
    gpio_set_dir(echo_pin, GPIO_IN);
    //---------------------------
    wrap_7 = set_frequency_7(FREQ_7);
    set_duty_7(DUTY_7, wrap_7);
    pwm_set_enabled(SLICE_NUM_7, false);
    sleep_ms(1000);
    //---------------------------

    // BTN
    const uint btn = 3;
    uint countsound = 0;

    // IR receive
    PIO pio = pio0;
    uint rx_gpio = 5;
    int rx_sm = nec_rx_init(pio, rx_gpio);
    uint8_t rx_address, rx_data;
    uint x_pos = 5, y_pos = 8;

    // OLED
    init_HW();

    if (rx_sm == -1)
    {
        printf("could not configure PIO\n");
        return -1;
    }

    // Initialize render area for entire frame (SSD1306_WIDTH pixels by SSD1306_NUM_PAGES pages)
    struct render_area frame_area = {
        start_col : 0,
        end_col : SSD1306_WIDTH - 1,
        start_page : 0,
        end_page : SSD1306_NUM_PAGES - 1
    };

    calc_render_area_buflen(&frame_area);

    // zero the entire display
    uint8_t buf[SSD1306_BUF_LEN];
    memset(buf, 0, SSD1306_BUF_LEN);
    render(buf, &frame_area);

    // Init LED pin Motor
    init_port(In1_pin, GPIO_OUT);
    init_port(In2_pin, GPIO_OUT);
    init_port(In3_pin, GPIO_OUT);
    init_port(In4_pin, GPIO_OUT);
    sleep_ms(100);

    // Init Devices
    init_soundSensor();
    initialize_pwm_channel(GPIO_DC);
    initBuzzer();
    init_port(btn, GPIO_IN);
    gpio_pull_up(btn);

    // Create Passwords
    char passwords[] = {1,
                        2,
                        3,
                        4};
    int mypw[] = {0};

    // DC MOTOR
    uint wrap;
    wrap = set_frequency(261);
    set_duty(0, wrap);
    pwm_set_enabled(SLICE_NUM, false);
    sleep_ms(100);

    while (true)
    {

        // IR receive
        while (!pio_sm_is_rx_fifo_empty(pio, rx_sm))
        {
            uint32_t rx_frame = pio_sm_get(pio, rx_sm);

            if (nec_decode_frame(rx_frame, &rx_address, &rx_data))
                printf("\treceived: %02x, %02x\n", rx_address, rx_data);
            else
                printf("\treceived: %08x\n", rx_frame);
        }
        WriteString(buf, 5, 0, "Enter Password");
        render(buf, &frame_area);

        if (rx_data == 0x16)
        {
            WriteString(buf, x_pos, y_pos, "0");
            x_pos += 8;
            onSMotor(1);
        }
        else if (rx_data == 0x15)
        {
            WriteString(buf, x_pos, y_pos, " ");
            x_pos += 8;
        }
        else if (rx_data == 0x0c)
        {
            WriteString(buf, x_pos, y_pos, "1");
            x_pos += 8;
            offSMotor(1);
        }
        else if (rx_data == 0x18)
        {
            WriteString(buf, x_pos, y_pos, "2");
            x_pos += 8;
        }
        else if (rx_data == 0x5e)
        {
            WriteString(buf, x_pos, y_pos, "3");
            x_pos += 8;
        }
        else if (rx_data == 0x08)
        {
            WriteString(buf, x_pos, y_pos, "4");
            x_pos += 8;
        }
        else if (rx_data == 0x1c)
        {
            WriteString(buf, x_pos, y_pos, "5");
            x_pos += 8;
        }
        else if (rx_data == 0x5a)
        {
            WriteString(buf, x_pos, y_pos, "6");
            x_pos += 8;
        }
        else if (rx_data == 0x42)
        {
            WriteString(buf, x_pos, y_pos, "7");
            x_pos += 8;
        }
        else if (rx_data == 0x52)
        {
            WriteString(buf, x_pos, y_pos, "8");
            x_pos += 8;
        }
        else if (rx_data == 0x4a)
        {
            WriteString(buf, x_pos, y_pos, "9");
            x_pos += 8;
        }

        // my code
        if (gpio_get(sound_pin) && (countsound < 3))
        {
            /* SSD1306_send_cmd(SSD1306_SET_ENTIRE_ON);
            char *text[] = {
                "vanhpheng",
                "    PICO"};

            int y = 0;
            for (int i = 0; i < count_of(text); i++)
            {
                WriteString(buf, 5, y, text[i]);
                y += 8;
            }
            // render(buf, &frame_area);
            x_pos += 40; */

            countsound++;
            sleep_ms(100);
        }
        int sw;
        if (countsound == 2)
        {
            sw = 1;
            gpio_put(led_pin, true);
            gpio_put(R_LED, true);
            gpio_put(G_LED, true);
            gpio_put(B_LED, true);
            sleep_ms(100);
            set_duty(90, wrap);
            sleep_ms(100);
            timeSong(1);
            sleep_ms(100);
            countsound++;
            sleep_ms(100);
        }
        if (sw == 1)
        {
            //---------ULTRA_SERVO------------
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
            sleep_ms(500);
            //--------------------------------
        }

        if (!gpio_get(btn))
        {
            sw = 0;
            countsound = 0;
            sleep_ms(100);
            gpio_put(led_pin, false);
            gpio_put(R_LED, false);
            gpio_put(G_LED, false);
            gpio_put(B_LED, false);
            sleep_ms(100);
            set_duty(0, wrap);
            sleep_ms(100);
        }

        // end of my code

        rx_data = 0;
        if (x_pos >= 120)
        {
            x_pos = 5;
            y_pos += 8;
        }
    }
    return 0;
}
