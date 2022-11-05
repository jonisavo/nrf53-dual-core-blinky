#ifndef __BUTTON_H
#define __BUTTON_H

#include <stdbool.h>
#include <drivers/gpio.h>

typedef enum button {
    BUTTON_0,
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
    BUTTON_MAX
} button_t;

void button_init(void);
void button_set_net_core_control(const button_t button);
void button_register_callback(const button_t button, gpio_callback_handler_t callback);
gpio_pin_t button_get_pin(const button_t button);

#endif // __BUTTON_H
