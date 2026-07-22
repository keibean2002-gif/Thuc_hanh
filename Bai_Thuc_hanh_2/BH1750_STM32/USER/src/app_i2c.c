#include "app_i2c.h"

/*=========================================================
 * HÀM CH? S? KI?N I2C CÓ TIMEOUT
 *=========================================================*/
static uint8_t I2C_WaitEvent(
    I2C_TypeDef *I2Cx,
    uint32_t I2C_EVENT
)
{
    uint32_t timeout = 100000;

    while (!I2C_CheckEvent(I2Cx, I2C_EVENT))
    {
        timeout--;

        if (timeout == 0)
        {
            return 0;
        }
    }

    return 1;
}


/*=========================================================
 * C?U HÌNH I2C1
 *
 * PB6 -> SCL
 * PB7 -> SDA
 *=========================================================*/
void I2C_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    /* B?t clock GPIOB, AFIO */
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOB |
        RCC_APB2Periph_AFIO,
        ENABLE
    );

    /* B?t clock I2C1 */
    RCC_APB1PeriphClockCmd(
        RCC_APB1Periph_I2C1,
        ENABLE
    );

    /*
     * PB6 -> I2C1_SCL
     * PB7 -> I2C1_SDA
     *
     * I2C s? d?ng Alternate Function Open Drain
     */
    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_6 |
        GPIO_Pin_7;

    GPIO_InitStructure.GPIO_Speed =
        GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Mode =
        GPIO_Mode_AF_OD;

    GPIO_Init(
        GPIOB,
        &GPIO_InitStructure
    );

    /* C?u hình I2C1 */
    I2C_InitStructure.I2C_Mode =
        I2C_Mode_I2C;

    I2C_InitStructure.I2C_DutyCycle =
        I2C_DutyCycle_2;

    I2C_InitStructure.I2C_OwnAddress1 =
        0x00;

    /*
     * Khi nh?n nhi?u byte,
     * ACK s? du?c b?t/t?t trong hàm d?c
     */
    I2C_InitStructure.I2C_Ack =
        I2C_Ack_Enable;

    I2C_InitStructure.I2C_AcknowledgedAddress =
        I2C_AcknowledgedAddress_7bit;

    I2C_InitStructure.I2C_ClockSpeed =
        100000;

    I2C_Init(
        I2C1,
        &I2C_InitStructure
    );

    /* Kích ho?t I2C1 */
    I2C_Cmd(
        I2C1,
        ENABLE
    );

    /* B?t ACK m?c d?nh */
    I2C_AcknowledgeConfig(
        I2C1,
        ENABLE
    );
}


/*=========================================================
 * G?I 1 L?NH Ð?N BH1750
 *
 * device_addr:
 *     0x23 ho?c 0x5C
 *
 * command:
 *     0x20 -> Continuous H-Resolution Mode
 *=========================================================*/
uint8_t I2C1_WriteCommand(
    uint8_t device_addr,
    uint8_t command
)
{
    /*
     * T?o START
     */
    I2C_GenerateSTART(
        I2C1,
        ENABLE
    );

    if (!I2C_WaitEvent(
            I2C1,
            I2C_EVENT_MASTER_MODE_SELECT
        ))
    {
        return 0;
    }

    /*
     * G?i d?a ch? thi?t b?
     *
     * BH1750:
     * 7-bit address = 0x23
     *
     * STM32 c?n d?a ch?:
     * 0x23 << 1 = 0x46
     */
    I2C_Send7bitAddress(
        I2C1,
        device_addr << 1,
        I2C_Direction_Transmitter
    );

    if (!I2C_WaitEvent(
            I2C1,
            I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED
        ))
    {
        I2C_GenerateSTOP(
            I2C1,
            ENABLE
        );

        return 0;
    }

    /*
     * G?i l?nh do ánh sáng
     */
    I2C_SendData(
        I2C1,
        command
    );

    if (!I2C_WaitEvent(
            I2C1,
            I2C_EVENT_MASTER_BYTE_TRANSMITTED
        ))
    {
        I2C_GenerateSTOP(
            I2C1,
            ENABLE
        );

        return 0;
    }

    /*
     * STOP
     */
    I2C_GenerateSTOP(
        I2C1,
        ENABLE
    );

    return 1;
}


/*=========================================================
 * Ð?C NHI?U BYTE T? BH1750
 *
 * BH1750 tr? v?:
 *
 * data[0] = MSB
 * data[1] = LSB
 *
 * T?ng c?ng 2 byte
 *=========================================================*/
uint8_t I2C1_ReadBuf(
    uint8_t device_addr,
    uint8_t *buffer,
    uint8_t size
)
{
    uint8_t i;

    if (size == 0)
    {
        return 0;
    }

    /*
     * T?o START
     */
    I2C_GenerateSTART(
        I2C1,
        ENABLE
    );

    if (!I2C_WaitEvent(
            I2C1,
            I2C_EVENT_MASTER_MODE_SELECT
        ))
    {
        return 0;
    }

    /*
     * G?i d?a ch? BH1750 ? ch? d? nh?n
     */
    I2C_Send7bitAddress(
        I2C1,
        device_addr << 1,
        I2C_Direction_Receiver
    );

    if (!I2C_WaitEvent(
            I2C1,
            I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED
        ))
    {
        I2C_GenerateSTOP(
            I2C1,
            ENABLE
        );

        return 0;
    }

    /*
     * Ð?c t?ng byte
     */
    for (i = 0; i < size; i++)
    {
        /*
         * N?u là byte cu?i cùng:
         * Không g?i ACK
         */
        if (i == size - 1)
        {
            I2C_AcknowledgeConfig(
                I2C1,
                DISABLE
            );

            /*
             * STOP tru?c khi d?c byte cu?i
             */
            I2C_GenerateSTOP(
                I2C1,
                ENABLE
            );
        }
        else
        {
            /*
             * Các byte tru?c:
             * G?i ACK d? ti?p t?c nh?n
             */
            I2C_AcknowledgeConfig(
                I2C1,
                ENABLE
            );
        }

        /*
         * Ch? nh?n du?c d? li?u
         */
        if (!I2C_WaitEvent(
                I2C1,
                I2C_EVENT_MASTER_BYTE_RECEIVED
            ))
        {
            /*
             * Khôi ph?c ACK
             */
            I2C_AcknowledgeConfig(
                I2C1,
                ENABLE
            );

            return 0;
        }

        /*
         * Ð?c d? li?u t? thanh ghi I2C
         */
        buffer[i] =
            I2C_ReceiveData(I2C1);
    }

    /*
     * B?t l?i ACK cho l?n d?c ti?p theo
     */
    I2C_AcknowledgeConfig(
        I2C1,
        ENABLE
    );

    return 1;
}