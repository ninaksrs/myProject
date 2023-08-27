
int main()
{
    const uint led_pin = 25;
    const uint sound_pin = 28;

    uint count = 0;

    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    gpio_init(sound_pin);
    gpio_set_dir(sound_pin, GPIO_IN);

    stdio_init_all();

    while (1)
    {
        if (gpio_get(sound_pin))
        {
            printf("[%05d] Sensing Sound~~!!\n", ++count);
            gpio_put(led_pin, 1);
            sleep_ms(1000);
        }
        else
        {
            gpio_put(led_pin, 0);
        }
    }

    return 0;
}
