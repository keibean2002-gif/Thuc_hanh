#ifndef __BMI160_H
#define __BMI160_H


#include "stm32f10x.h"
#include <stdint.h>


/*
====================================================
CHIP ID
====================================================
*/

#define BMI160_CHIP_ID       0x00

#define BMI160_ID            0xD1


/*
====================================================
GYROSCOPE DATA
====================================================
*/

#define BMI160_GYR_X_L       0x0C
#define BMI160_GYR_X_H       0x0D

#define BMI160_GYR_Y_L       0x0E
#define BMI160_GYR_Y_H       0x0F

#define BMI160_GYR_Z_L       0x10
#define BMI160_GYR_Z_H       0x11


/*
====================================================
ACCELEROMETER DATA
====================================================
*/

#define BMI160_ACC_X_L       0x12
#define BMI160_ACC_X_H       0x13

#define BMI160_ACC_Y_L       0x14
#define BMI160_ACC_Y_H       0x15

#define BMI160_ACC_Z_L       0x16
#define BMI160_ACC_Z_H       0x17


/*
====================================================
CONFIGURATION
====================================================
*/

#define BMI160_CMD_REG       0x7E

#define BMI160_ACC_CONF      0x40
#define BMI160_ACC_RANGE     0x41

#define BMI160_GYR_CONF      0x42
#define BMI160_GYR_RANGE     0x43


/*
====================================================
KI?U D? LI?U 3 TR?C
====================================================
*/

typedef struct
{
    int16_t x;

    int16_t y;

    int16_t z;

} BMI160_Data;


/*
====================================================
KHAI BÁO HÀM
====================================================
*/

uint8_t BMI160_ReadChipID(
    void
);


uint8_t BMI160_Init(
    void
);


void BMI160_ReadAccel(
    BMI160_Data *accel
);


void BMI160_ReadGyro(
    BMI160_Data *gyro
);


#endif