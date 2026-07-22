#include "delay.h"


/*
====================================================
KH?I T?O DELAY
====================================================
*/

void Delay_Init(void)
{
    /*
     * Không c?n c?u hình thêm
     *
     * S? d?ng SystemCoreClock
     */
}


/*
====================================================
DELAY MILLISECOND
====================================================
*/

void Delay_ms(
    uint32_t ms
)
{
    uint32_t i;


    for(
        i = 0;
        i < ms;
        i++
    )
    {
        /*
         * T?o delay 1 ms
         */

        SysTick->LOAD =
            (SystemCoreClock / 1000)
            - 1;


        /*
         * Xóa giá tr? hi?n t?i
         */

        SysTick->VAL = 0;


        /*
         * B?t SysTick
         */

        SysTick->CTRL =
            SysTick_CTRL_CLKSOURCE_Msk |
            SysTick_CTRL_ENABLE_Msk;


        /*
         * Ch? d?m xong
         */

        while(
            (
                SysTick->CTRL &
                SysTick_CTRL_COUNTFLAG
            )
            == 0
        );


        /*
         * T?t SysTick
         */

        SysTick->CTRL = 0;
    }
}