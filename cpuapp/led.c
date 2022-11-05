#include "led.h"

#include <zephyr/logging/log.h>
#include <drivers/gpio.h>

LOG_MODULE_REGISTER(led);

#define LED0_NODE DT_NODELABEL(led0)
#define LED1_NODE DT_NODELABEL(led1)
#define LED2_NODE DT_NODELABEL(led2)
#define LED3_NODE DT_NODELABEL(led3)

static const struct gpio_dt_spec leds[] = {
    GPIO_DT_SPEC_GET(LED0_NODE, gpios),
    GPIO_DT_SPEC_GET(LED1_NODE, gpios),
    GPIO_DT_SPEC_GET(LED2_NODE, gpios),
    GPIO_DT_SPEC_GET(LED3_NODE, gpios)
};

/**
 * @brief Initializes the LEDs.
 */
void led_init(void)
{
    LOG_DBG("Initializing LEDs");

    for (size_t i = 0; i < ARRAY_SIZE(leds); i++)
    {
        gpio_pin_configure_dt(&leds[i], GPIO_OUTPUT);
        gpio_pin_set_dt(&leds[i], 0);
    }
}

/**
 * @brief Sets the state of the LED.
 * @param led LED to set.
 * @param state State to set the LED to.
 */
void led_set(led_t led, const bool state)
{
    LOG_DBG("Setting LED %d to %d", led, state);

    __ASSERT(led < LED_MAX, "Invalid LED: %d", led);

    gpio_pin_set_dt(&leds[led], state);
}

/**
 * @brief Sets the state of all LEDs.
 * @param state State to set.
 */
void led_set_all(const bool state)
{
    LOG_DBG("Setting all LEDs to %d", state);

    for (size_t i = 0; i < ARRAY_SIZE(leds); i++)
        gpio_pin_set_dt(&leds[i], state);
}
