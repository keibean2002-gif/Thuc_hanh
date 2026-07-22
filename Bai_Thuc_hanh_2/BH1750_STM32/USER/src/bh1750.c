#include "bh1750.h"

void BH1750_Init(void)
{
    I2C_Config();
}


float BH1750_ReadLux(void)
{
    uint8_t data[2];
    uint16_t raw;
    float lux;


    /*
     * G?i l?nh do
     *
     * 0x20:
     * Continuous High Resolution Mode
     */
    if (
        I2C1_WriteCommand(
            BH1750_ADDR,
            BH1750_ONE_H_MODE
        )
        == 0
    )
    {
        return -1.0f;
    }


    /*
     * Ch? BH1750 do xong
     */
    Delay_ms(180);


    /*
     * Ð?c 2 byte d? li?u
     */
    if (
        I2C1_ReadBuf(
            BH1750_ADDR,
            data,
            2
        )
        == 0
    )
    {
        return -1.0f;
    }


    /*
     * Ghép 2 byte:
     *
     * data[0] = MSB
     * data[1] = LSB
     */
    raw =
        ((uint16_t)data[0] << 8)
        |
        data[1];


    /*
     * Ð?i sang lux
     *
     * Lux = Raw / 1.2
     */
    lux =
        (float)raw / 1.2f;


    return lux;
}