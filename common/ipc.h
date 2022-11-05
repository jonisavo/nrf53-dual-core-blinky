#ifndef __IPC_H
#define __IPC_H

#include <stdint.h>
#include <zephyr/ipc/ipc_service.h>

typedef enum icmsg_type
{
    ICMSG_TYPE_NONE = 0,
    ICMSG_TYPE_BUTTON,
    ICMSG_TYPE_MAX
} icmsg_type_t;

/**
 * @brief Message structure for inter-core communication.
 */
typedef struct icmsg
{
    uint8_t type;
    union {
        uint32_t data;
        uint8_t bytes[4];
    };
} icmsg_t;

void ipc_init(const struct ipc_ept_cfg* const config);
void icmsg_send(const icmsg_t* const message);

#endif
