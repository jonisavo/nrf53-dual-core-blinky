#ifndef __GPIO_H
#define __GPIO_H

#include <drivers/gpio.h>

/**
 * @brief Possible pin control modes.
 */
typedef enum pin_control {
    PIN_CONTROL_APP_CORE,
    PIN_CONTROL_NETWORK_CORE,
    PIN_CONTROL_MAX
} pin_control_t;

void gpio_set_pin_control(const gpio_pin_t pin, const pin_control_t pin_control);

#endif
