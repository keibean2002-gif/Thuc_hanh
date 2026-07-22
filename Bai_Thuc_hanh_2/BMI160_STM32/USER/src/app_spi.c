#include "app_spi.h"


/*
====================================================
K?T N?I SPI1
====================================================

BMI160       STM32F103

CS     --->   PA4
SCL    --->   PA5 - SPI1_SCK
SDA    <---   PA6 - SPI1_MISO
SDX    --->   PA7 - SPI1_MOSI

====================================================
*/


#define BMI160_CS_PORT    GPIOA
#define BMI160_CS_PIN     GPIO_Pin_4


/*
====================================================
CS LOW
====================================================
*/

void BMI160_CS_Low(void)
{
    GPIO_ResetBits(
        BMI160_CS_PORT,
        BMI160_CS_PIN
    );
}


/*
====================================================
CS HIGH
====================================================
*/

void BMI160_CS_High(void)
{
    GPIO_SetBits(
        BMI160_CS_PORT,
        BMI160_CS_PIN
    );
}


/*
====================================================
C?U HÌNH SPI1
====================================================
*/
void SPI1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;


    /*
     * B?t clock
     */
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOA |
        RCC_APB2Periph_AFIO |
        RCC_APB2Periph_SPI1,
        ENABLE
    );


    /*
     * PA5 - SCK
     * PA7 - MOSI
     */
    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_5 |
        GPIO_Pin_7;

    GPIO_InitStructure.GPIO_Mode =
        GPIO_Mode_AF_PP;

    GPIO_InitStructure.GPIO_Speed =
        GPIO_Speed_50MHz;

    GPIO_Init(
        GPIOA,
        &GPIO_InitStructure
    );


    /*
     * PA6 - MISO
     */
    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_6;

    GPIO_InitStructure.GPIO_Mode =
        GPIO_Mode_IN_FLOATING;

    GPIO_Init(
        GPIOA,
        &GPIO_InitStructure
    );


    /*
     * PA4 - CS
     */
    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_4;

    GPIO_InitStructure.GPIO_Mode =
        GPIO_Mode_Out_PP;

    GPIO_InitStructure.GPIO_Speed =
        GPIO_Speed_50MHz;

    GPIO_Init(
        GPIOA,
        &GPIO_InitStructure
    );


    /*
     * CS HIGH
     */
    GPIO_SetBits(
        GPIOA,
        GPIO_Pin_4
    );


    /*
     * SPI c?u hình
     */
    SPI_InitStructure.SPI_Direction =
        SPI_Direction_2Lines_FullDuplex;

    SPI_InitStructure.SPI_Mode =
        SPI_Mode_Master;

    SPI_InitStructure.SPI_DataSize =
        SPI_DataSize_8b;

    SPI_InitStructure.SPI_CPOL =
        SPI_CPOL_Low;

    SPI_InitStructure.SPI_CPHA =
        SPI_CPHA_1Edge;

    SPI_InitStructure.SPI_NSS =
        SPI_NSS_Soft;

    SPI_InitStructure.SPI_BaudRatePrescaler =
        SPI_BaudRatePrescaler_16;

    SPI_InitStructure.SPI_FirstBit =
        SPI_FirstBit_MSB;

    SPI_InitStructure.SPI_CRCPolynomial =
        7;


    SPI_Init(
        SPI1,
        &SPI_InitStructure
    );


    SPI_Cmd(
        SPI1,
        ENABLE
    );
}


/*
====================================================
G?I VÀ NH?N 1 BYTE
====================================================
*/
uint8_t SPI1_ReadWriteByte(uint8_t data)
{
    uint32_t timeout;


    timeout = 100000;

    /*
     * Xóa d? li?u cu n?u có
     */
    while(
        SPI_I2S_GetFlagStatus(
            SPI1,
            SPI_I2S_FLAG_RXNE
        ) == SET
    )
    {
        SPI_I2S_ReceiveData(SPI1);
    }


    /*
     * Ch? b? d?m truy?n r?ng
     */
    while(
        SPI_I2S_GetFlagStatus(
            SPI1,
            SPI_I2S_FLAG_TXE
        ) == RESET
    )
    {
        timeout--;

        if(timeout == 0)
        {
            return 0xFF;
        }
    }


    /*
     * G?i d? li?u
     */
    SPI_I2S_SendData(
        SPI1,
        data
    );


    timeout = 100000;


    /*
     * Ch? nh?n d? li?u
     */
    while(
        SPI_I2S_GetFlagStatus(
            SPI1,
            SPI_I2S_FLAG_RXNE
        ) == RESET
    )
    {
        timeout--;

        if(timeout == 0)
        {
            return 0xFF;
        }
    }


    return SPI_I2S_ReceiveData(SPI1);
}


/*
====================================================
GHI 1 THANH GHI BMI160
====================================================
*/

void SPI1_WriteRegister(
    uint8_t reg,
    uint8_t data
)
{
    /*
     * Bit 7 = 0
     * => WRITE
     */

    reg &= 0x7F;


    BMI160_CS_Low();


    /*
     * G?i d?a ch? thanh ghi
     */

    SPI1_ReadWriteByte(
        reg
    );


    /*
     * G?i d? li?u
     */

    SPI1_ReadWriteByte(
        data
    );


    BMI160_CS_High();
}


/*
====================================================
Ð?C 1 THANH GHI BMI160
====================================================
*/
uint8_t SPI1_ReadRegister(uint8_t reg)
{
    uint8_t data;

    BMI160_CS_Low();

    Delay_ms(1);

    /*
     * Ð?a ch? d?c:
     * bit 7 = 1
     */
    SPI1_ReadWriteByte(reg | 0x80);

    /*
     * BMI160 c?n m?t dummy byte
     */
    data = SPI1_ReadWriteByte(0x00);

    Delay_ms(1);

    BMI160_CS_High();

    return data;
}


/*
====================================================
Ð?C NHI?U BYTE
====================================================
*/

void SPI1_ReadBuffer(
    uint8_t reg,
    uint8_t *buffer,
    uint8_t length
)
{
    uint8_t i;


    /*
     * Bit 7 = 1
     * => READ
     */

    reg |= 0x80;


    BMI160_CS_Low();


    /*
     * G?i d?a ch?
     */

    SPI1_ReadWriteByte(
        reg
    );


    /*
     * Dummy byte sau d?a ch?
     */

    SPI1_ReadWriteByte(
        0x00
    );


    /*
     * Ð?c d? li?u
     */

    for(
        i = 0;
        i < length;
        i++
    )
    {
        buffer[i] =
            SPI1_ReadWriteByte(
                0x00
            );
    }


    BMI160_CS_High();
}