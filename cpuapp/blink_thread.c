#include "blink_thread.h"
#include "led.h"

#include <zephyr/kernel.h>

#define BLINK_STACK_SIZE 1024

K_THREAD_STACK_DEFINE(blink_stack_area, BLINK_STACK_SIZE);
static struct k_thread blink_thread_data;
static k_tid_t blink_thread_id;

static inline void pause()
{
	k_msleep(250);
}

static void blink_thread(void* arg1, void* arg2, void* arg3)
{
	ARG_UNUSED(arg1);
	ARG_UNUSED(arg2);
	ARG_UNUSED(arg3);

	while (true)
	{
		for (int i = 0; i < 5; i++)
		{
			led_set(LED_0, true);
			pause();
			led_set(LED_0, false);
			led_set(LED_1, true);
			pause();
			led_set(LED_1, false);
			led_set(LED_3, true);
			pause();
			led_set(LED_3, false);
			led_set(LED_2, true);
			pause();
			led_set(LED_2, false);
		}

		for (int i = 0; i < 5; i++)
		{
			pause();
			led_set_all(true);
			pause();
			led_set_all(false);
		}
	}
}

void blink_thread_create()
{
	blink_thread_id = k_thread_create(&blink_thread_data, 
		blink_stack_area,
		BLINK_STACK_SIZE,
		blink_thread, NULL, NULL, NULL,
		K_PRIO_COOP(7), 0, K_NO_WAIT);

    k_thread_name_set(blink_thread_id, "Blink thread");
}

inline void blink_thread_suspend()
{
    __ASSERT(blink_thread_id != NULL, "Blink thread not created");

    k_thread_suspend(blink_thread_id);
}

inline void blink_thread_resume()
{
    __ASSERT(blink_thread_id != NULL, "Blink thread not created");

    k_thread_resume(blink_thread_id);
}
