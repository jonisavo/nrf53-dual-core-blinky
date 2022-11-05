#include "ipc.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <devicetree.h>

LOG_MODULE_REGISTER(ipc);

static struct ipc_ept hci_endpoint;

static const struct device* ipc_dev = DEVICE_DT_GET(DT_NODELABEL(ipc0));

/**
 * @brief Initializes the IPC.
 * @param config IPC configuration.
 */
void ipc_init(const struct ipc_ept_cfg* const config)
{
    LOG_INF("Initializing IPC");

    int err = ipc_service_open_instance(ipc_dev);

    __ASSERT(err == 0, "Failed to open IPC instance: %d", err);

    k_msleep(1000);

    err = ipc_service_register_endpoint(ipc_dev, &hci_endpoint, config);
    __ASSERT(err == 0, "Failed to register endpoint: %d", err);
}

/**
 * @brief Sends a message over IPC.
 * @param data The message to send.
 */
void icmsg_send(const icmsg_t* const data)
{
    LOG_HEXDUMP_INF(data, sizeof(icmsg_t), "Sending message");

    int err = ipc_service_send(&hci_endpoint, data, sizeof(icmsg_t));

    __ASSERT(err == 0, "Failed to send data: %d", err);
}
