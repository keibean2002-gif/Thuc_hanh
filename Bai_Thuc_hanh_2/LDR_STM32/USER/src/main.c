#include "stm32f10x.h"
#include "ldr.h"
#include "delay.h"

volatile uint8_t ldr_state;
const char *ldr_status;

int main(void)
{
    Delay_Init();

    LDR_Init();

    while (1)
    {
        
        ldr_state = LDR_Read();

        if (ldr_state == 1)
        {
            ldr_status = "Khong co anh sang";
        }
        else
        {
            ldr_status = "Co anh sang";
        }

        Delay_ms(500);
    }
}