#ifndef __BLINK_THREAD_H
#define __BLINK_THREAD_H

#include <zephyr/kernel.h>

void blink_thread_create();
void blink_thread_suspend();
void blink_thread_resume();

#endif
