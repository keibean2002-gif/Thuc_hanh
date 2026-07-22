#include "ldr.h"

void LDR_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOA,
        ENABLE
    );

    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t LDR_Read(void)
{
    return GPIO_ReadInputDataBit(
        GPIOA,
        GPIO_Pin_0
    );
}