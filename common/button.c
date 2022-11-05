#include "button.h"

#include <zephyr/logging/log.h>
#include <devicetree.h>
#include "gpio.h"

LOG_MODULE_REGISTER(button);

#define BUTTON0_NODE DT_NODELABEL(button0)
#define BUTTON1_NODE DT_NODELABEL(button1)
#define BUTTON2_NODE DT_NODELABEL(button2)
#define BUTTON3_NODE DT_NODELABEL(button3)

static const struct gpio_dt_spec buttons[BUTTON_MAX] = {
    GPIO_DT_SPEC_GET(BUTTON0_NODE, gpios),
    GPIO_DT_SPEC_GET(BUTTON1_NODE, gpios),
    GPIO_DT_SPEC_GET(BUTTON2_NODE, gpios),
    GPIO_DT_SPEC_GET(BUTTON3_NODE, gpios)
};

static struct gpio_callback button_callbacks[BUTTON_MAX];

void button_init(void)
{
    LOG_INF("Initializing buttons");

    for (size_t i = 0; i < BUTTON_MAX; i++)
    {
        gpio_pin_configure_dt(&buttons[i], GPIO_INPUT);
        button_callbacks[i] = (struct gpio_callback) {};
    }
}

static const inline struct gpio_dt_spec* get_button_spec(const button_t button)
{
    __ASSERT(button < BUTTON_MAX, "Invalid button: %d", button);

    return &buttons[button];
}

static const inline struct gpio_callback* get_button_callback(const button_t button)
{
    __ASSERT(button < BUTTON_MAX, "Invalid button: %d", button);

    return &button_callbacks[button];
}

/**
 * @brief Gives ownership of a button to the nrf53 network core.
 */
void button_set_net_core_control(const button_t button)
{
    const struct gpio_dt_spec* button_spec = get_button_spec(button);

    gpio_set_pin_control(button_spec->pin, PIN_CONTROL_NETWORK_CORE);
}

/**
 * @brief Registers a callback for a button.
 */
void button_register_callback(const button_t button, gpio_callback_handler_t callback)
{
    LOG_INF("Registering callback for button %d", button);

    const struct gpio_dt_spec* button_spec = get_button_spec(button);
    const struct gpio_callback* callback_data = get_button_callback(button);

    int ret = 0;

    ret = gpio_pin_interrupt_configure_dt(button_spec, GPIO_INT_EDGE_BOTH | GPIO_INT_DEBOUNCE);

    __ASSERT(ret == 0, "Configuring pin interrupt failed: %d", ret);

    gpio_init_callback(callback_data, callback, BIT(button_spec->pin));

    ret = gpio_add_callback(button_spec->port, callback_data);

    __ASSERT(ret == 0, "Adding callback failed: %d", ret);

    LOG_INF("Callback registered for button %d, port %d, pin %d", button, button_spec->port, button_spec->pin);
}

/**
 * @brief Gets the pin of a button. Returns 0xFF if the button is invalid.
 */
gpio_pin_t button_get_pin(const button_t button)
{
    gpio_pin_t pin = 0xff;

    const struct gpio_dt_spec* button_spec = get_button_spec(button);

    if (button_spec != NULL)
        pin = button_spec->pin;

    return pin;
}
