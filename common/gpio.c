#include "gpio.h"

#include <zephyr/logging/log.h>
#include <devicetree.h>

LOG_MODULE_REGISTER(gpio);

// https://infocenter.nordicsemi.com/index.jsp?topic=%2Fug_nrf5340_dk%2FUG%2Fdk%2Fintro.html
#define GPIO_SECURITY_ADDRESS 0x50842500

// https://infocenter.nordicsemi.com/index.jsp?topic=%2Fug_nrf5340_dk%2FUG%2Fdk%2Fintro.html
#define PIN_CNF_ADDR_OFFSET 0x200
#define PIN_CNF_MCUSEL_BIT 28

/**
 * @brief Set the pin control for the given pin.
 * @param pin The pin to set the control for.
 * @param pin_control The pin control to set.
 */
void gpio_set_pin_control(const gpio_pin_t pin, const pin_control_t pin_control)
{
    __ASSERT(pin_control < PIN_CONTROL_MAX, "Invalid pin control: %d", pin_control);

    const uint32_t pin_offset = pin * sizeof(uint32_t);

    const uint32_t pin_config_address =
        GPIO_SECURITY_ADDRESS + PIN_CNF_ADDR_OFFSET + pin_offset;

    uint32_t* const config_address = (uint32_t*)pin_config_address;

    LOG_DBG("PIN_CNF address for %d: %p", pin, config_address);

    LOG_HEXDUMP_DBG(config_address, sizeof(uint32_t), "Old PIN_CNF value");

    if (pin_control == PIN_CONTROL_APP_CORE)
        *config_address &= ~(1 << PIN_CNF_MCUSEL_BIT);  // Set MCUSEL to 0
    else
        *config_address |= (1 << PIN_CNF_MCUSEL_BIT);   // Set MCUSEL to 1

    LOG_HEXDUMP_DBG(config_address, sizeof(uint32_t), "New PIN_CNF value");
}
