#include "uart.h"

void UART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /*
     * B?t clock cho:
     * GPIOA
     * USART1
     */
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOA |
        RCC_APB2Periph_USART1,
        ENABLE
    );

    /*
     * PA9 - USART1_TX
     */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_Init(
        GPIOA,
        &GPIO_InitStructure
    );

    /*
     * PA10 - USART1_RX
     */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    GPIO_Init(
        GPIOA,
        &GPIO_InitStructure
    );

    /*
     * C?u hình USART1
     */
    USART_InitStructure.USART_BaudRate = 9600;

    USART_InitStructure.USART_WordLength =
        USART_WordLength_8b;

    USART_InitStructure.USART_StopBits =
        USART_StopBits_1;

    USART_InitStructure.USART_Parity =
        USART_Parity_No;

    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;

    USART_InitStructure.USART_Mode =
        USART_Mode_Tx |
        USART_Mode_Rx;

    USART_Init(
        USART1,
        &USART_InitStructure
    );

    /*
     * B?t USART1
     */
    USART_Cmd(
        USART1,
        ENABLE
    );
}


/*
 * G?i 1 ký t?
 */
void UART1_SendChar(char c)
{
    USART_SendData(
        USART1,
        c
    );

    while (
        USART_GetFlagStatus(
            USART1,
            USART_FLAG_TXE
        ) == RESET
    );
}


/*
 * G?i chu?i
 */
void UART1_SendString(char *str)
{
    while (*str)
    {
        UART1_SendChar(*str);
        str++;
    }
}


/*
 * Hàm printf s? d?ng USART1
 */
int fputc(
    int ch,
    FILE *f
)
{
    UART1_SendChar((char)ch);

    return ch;
}