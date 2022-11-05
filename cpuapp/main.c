#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/logging/log_ctrl.h>
#include "led.h"
#include "blink_thread.h"
#include "../common/gpio.h"
#include "../common/ipc.h"
#include "../common/button.h"

LOG_MODULE_REGISTER(application);

static K_SEM_DEFINE(bound_sem, 0, 1);

static void ipc_endpoint_bound(void* priv)
{
	k_sem_give(&bound_sem);
	LOG_INF("IPC endpoint bound");
}

static void ipc_endpoint_received(const void *data, size_t len, void *priv)
{
	ARG_UNUSED(priv);

	const icmsg_t* message = (icmsg_t*)data;

	if (message->type != ICMSG_TYPE_BUTTON)
	{
		LOG_WRN("Invalid message type: %d", message->type);
		LOG_HEXDUMP_WRN(data, len, "Raw data");
		return;
	}

	LOG_INF("Received button %d message (type %x) from CPU1", message->bytes[0], message->type);

	const bool is_pressed = message->bytes[1] == 1;

	LOG_INF("Button is %s", is_pressed ? "pressed" : "released");
	LOG_HEXDUMP_INF(data, len, "Raw data");

	if (is_pressed)
	{
		blink_thread_suspend();
		led_set_all(false);
		led_set(message->bytes[0], true);
	}
	else
	{
		led_set_all(false);
		blink_thread_resume();
	}
}

static struct ipc_ept_cfg ep_cfg = {
	.name = "ipc-app",
	.cb = {
		.bound = ipc_endpoint_bound,
		.received = ipc_endpoint_received,
	},
};

int main(void)
{
	log_init();

	LOG_INF("Hello world from %s", CONFIG_BOARD);

	ipc_init(&ep_cfg);

	button_init();
	button_set_net_core_control(BUTTON_0);
	button_set_net_core_control(BUTTON_1);
	button_set_net_core_control(BUTTON_2);
	button_set_net_core_control(BUTTON_3);

	led_init();

	k_sem_take(&bound_sem, K_FOREVER);

	blink_thread_create();

	k_sleep(K_FOREVER);

	return 0;
}
