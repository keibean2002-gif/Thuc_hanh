#include "stm32f10x.h"
#include "delay.h"
#include "app_spi.h"
#include "bmi160.h"


BMI160_Data accel;
BMI160_Data gyro;

uint8_t chip_id;
uint8_t bmi160_status;


int main(void)
{
    Delay_Init();

    SPI1_Config();

    Delay_ms(100);


    /*
     * Ð?c CHIP ID
     */
    chip_id = BMI160_ReadChipID();


    /*
     * D?ng t?i dây d? ki?m tra chip_id
     */
    while(1)
    {
        Delay_ms(1000);
    }
}