#include "stm32f10x.h"
#include "bh1750.h"
#include "delay.h"

volatile float lux;

int main(void)
{
    BH1750_Init();

    while (1)
    {
        lux = BH1750_ReadLux();

        Delay_ms(500);
    }
}