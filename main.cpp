#include "pico/stdlib.h"
#include "mcp2515.h"
#include "MS5837.h"

void init() {
    
}

int main()
{
    const uint LED_PIN = 25;
    const uint RADIO_BUTTON_PIN = 2;
    gpio_init(LED_PIN);
    gpio_init(RADIO_BUTTON_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(RADIO_BUTTON_PIN, GPIO_IN);

    while (true) {
        bool led_on = gpio_get(RADIO_BUTTON_PIN);

        if (led_on) {
            gpio_put(LED_PIN, 1);
        } else {
            gpio_put(LED_PIN, 0);
        }

    }
}

