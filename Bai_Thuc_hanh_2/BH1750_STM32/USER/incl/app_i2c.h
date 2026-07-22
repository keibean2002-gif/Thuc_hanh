#ifndef __APP_I2C_H
#define __APP_I2C_H

#include "define.h"

void I2C_Config(void);

uint8_t I2C1_WriteCommand(
    uint8_t device_addr,
    uint8_t command
);

uint8_t I2C1_ReadBuf(
    uint8_t device_addr,
    uint8_t *buffer,
    uint8_t size
);

#endif