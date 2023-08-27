
const uint led_pin = 25;
const uint sound_pin = 28;
const uint R_LED = 22;
const uint G_LED = 26;
const uint B_LED = 27;

void init_soundSensor()
{
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    gpio_init(R_LED);
    gpio_set_dir(R_LED, GPIO_OUT);
    gpio_init(G_LED);
    gpio_set_dir(G_LED, GPIO_OUT);
    gpio_init(B_LED);
    gpio_set_dir(B_LED, GPIO_OUT);
    gpio_init(sound_pin);
    gpio_set_dir(sound_pin, GPIO_IN);
}