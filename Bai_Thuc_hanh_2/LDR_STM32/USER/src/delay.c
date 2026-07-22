#include "delay.h"

volatile uint32_t delay_ms_count;

void Delay_Init(void)
{
    
    SysTick_Config(SystemCoreClock / 1000);
}

void Delay_1ms(void)
{
    delay_ms_count = 1;

    while (delay_ms_count != 0);
}

void Delay_ms(uint32_t ms)
{
    delay_ms_count = ms;

    while (delay_ms_count != 0);
}


void SysTick_Handler(void)
{
    if (delay_ms_count > 0)
    {
        delay_ms_count--;
    }
}