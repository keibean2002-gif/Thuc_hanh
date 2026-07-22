#include "bmi160.h"
#include "app_spi.h"
#include "delay.h"


/*
====================================================
Ğ?C CHIP ID
====================================================
*/

uint8_t BMI160_ReadChipID(
    void
)
{
    return SPI1_ReadRegister(
        BMI160_CHIP_ID
    );
}


/*
====================================================
KH?I T?O BMI160
====================================================
*/

uint8_t BMI160_Init(
    void
)
{
    uint8_t chip_id;


    /*
     * Ğ?c Chip ID
     */

    chip_id =
        BMI160_ReadChipID();


    /*
     * Ki?m tra Chip ID
     */

    if(
        chip_id != BMI160_ID
    )
    {
        return 0;
    }


    /*
     * B?t Accelerometer
     *
     * 0x11
     * Normal Mode
     */

    SPI1_WriteRegister(
        BMI160_CMD_REG,
        0x11
    );


    Delay_ms(
        10
    );


    /*
     * Accelerometer:
     *
     * 100 Hz
     * Normal bandwidth
     */

    SPI1_WriteRegister(
        BMI160_ACC_CONF,
        0x28
    );


    /*
     * Accelerometer:
     *
     * ±2g
     */

    SPI1_WriteRegister(
        BMI160_ACC_RANGE,
        0x03
    );


    /*
     * B?t Gyroscope
     *
     * 0x15
     * Normal Mode
     */

    SPI1_WriteRegister(
        BMI160_CMD_REG,
        0x15
    );


    Delay_ms(
        100
    );


    /*
     * Gyroscope:
     *
     * 100 Hz
     * Normal bandwidth
     */

    SPI1_WriteRegister(
        BMI160_GYR_CONF,
        0x28
    );


    /*
     * Gyroscope:
     *
     * ±2000 deg/s
     */

    SPI1_WriteRegister(
        BMI160_GYR_RANGE,
        0x00
    );


    return 1;
}


/*
====================================================
Ğ?C ACCELEROMETER
====================================================
*/

void BMI160_ReadAccel(
    BMI160_Data *accel
)
{
    uint8_t data[6];


    /*
     * Ğ?c 6 byte:
     *
     * X_L
     * X_H
     * Y_L
     * Y_H
     * Z_L
     * Z_H
     */

    SPI1_ReadBuffer(
        BMI160_ACC_X_L,
        data,
        6
    );


    /*
     * Ghép X
     */

    accel->x =
        (int16_t)
        (
            ((uint16_t)data[1] << 8)
            |
            data[0]
        );


    /*
     * Ghép Y
     */

    accel->y =
        (int16_t)
        (
            ((uint16_t)data[3] << 8)
            |
            data[2]
        );


    /*
     * Ghép Z
     */

    accel->z =
        (int16_t)
        (
            ((uint16_t)data[5] << 8)
            |
            data[4]
        );
}


/*
====================================================
Ğ?C GYROSCOPE
====================================================
*/

void BMI160_ReadGyro(
    BMI160_Data *gyro
)
{
    uint8_t data[6];


    /*
     * Ğ?c 6 byte:
     *
     * X_L
     * X_H
     * Y_L
     * Y_H
     * Z_L
     * Z_H
     */

    SPI1_ReadBuffer(
        BMI160_GYR_X_L,
        data,
        6
    );


    /*
     * Ghép X
     */

    gyro->x =
        (int16_t)
        (
            ((uint16_t)data[1] << 8)
            |
            data[0]
        );


    /*
     * Ghép Y
     */

    gyro->y =
        (int16_t)
        (
            ((uint16_t)data[3] << 8)
            |
            data[2]
        );


    /*
     * Ghép Z
     */

    gyro->z =
        (int16_t)
        (
            ((uint16_t)data[5] << 8)
            |
            data[4]
        );
}