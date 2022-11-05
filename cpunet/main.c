#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/logging/log_ctrl.h>
#include "../common/button.h"
#include "../common/ipc.h"

LOG_MODULE_REGISTER(net);

static K_SEM_DEFINE(bound_sem, 0, 1);

static void ipc_endpoint_bound(void* priv)
{
	k_sem_give(&bound_sem);
	LOG_INF("IPC endpoint bound");
}

static void ipc_endpoint_received(const void *data, size_t len, void *priv)
{
	LOG_HEXDUMP_WRN(data, len, "Received message... from where?!");
}

static struct ipc_ept_cfg ep_cfg = {
	.name = "ipc-net",
	.cb = {
		.bound = ipc_endpoint_bound,
		.received = ipc_endpoint_received,
	},
};

static icmsg_t current_message = {
    .type = ICMSG_TYPE_NONE,
    .data = 0,
};

static void handle_button_press(const struct device* dev, const button_t button)
{
    const gpio_pin_t pin = button_get_pin(button);
    const int state = gpio_pin_get(dev, pin);
    LOG_INF("Button %d press event: %s", button, state ? "pressed" : "released");
    current_message.type = ICMSG_TYPE_BUTTON;
    current_message.data = 0;
    current_message.bytes[0] = button;
    current_message.bytes[1] = state;
    icmsg_send(&current_message);
}

static void button0_callback(const struct device* dev, struct gpio_callback* cb, gpio_port_pins_t pins)
{
    handle_button_press(dev, BUTTON_0);
}

static void button1_callback(const struct device* dev, struct gpio_callback* cb, gpio_port_pins_t pins)
{
    handle_button_press(dev, BUTTON_1);
}

static void button2_callback(const struct device* dev, struct gpio_callback* cb, gpio_port_pins_t pins)
{
    handle_button_press(dev, BUTTON_2);
}

static void button3_callback(const struct device* dev, struct gpio_callback* cb, gpio_port_pins_t pins)
{
    handle_button_press(dev, BUTTON_3);
}

int main(void)
{
    log_init();

	LOG_INF("Hello world from %s", CONFIG_BOARD);

    ipc_init(&ep_cfg);
    button_init();

    k_sem_take(&bound_sem, K_FOREVER);

    button_register_callback(BUTTON_0, button0_callback);
    button_register_callback(BUTTON_1, button1_callback);
    button_register_callback(BUTTON_2, button2_callback);
    button_register_callback(BUTTON_3, button3_callback);

    k_sleep(K_FOREVER);

    return 0;
}
