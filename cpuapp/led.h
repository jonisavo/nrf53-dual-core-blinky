#ifndef __LED_H
#define __LED_H

#include <stdbool.h>

typedef enum led {
    LED_0,
    LED_1,
    LED_2,
    LED_3,
    LED_MAX
} led_t;

void led_init(void);
void led_set(led_t led, const bool state);
void led_set_all(const bool state);

#endif // __LED_H
