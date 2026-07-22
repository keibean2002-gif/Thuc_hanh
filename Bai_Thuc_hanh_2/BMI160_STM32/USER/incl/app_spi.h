#ifndef __APP_SPI_H
#define __APP_SPI_H


#include "stm32f10x.h"
#include <stdint.h>


/* Kh?i t?o SPI1 */

void SPI1_Config(void);


/* G?i và nh?n 1 byte */

uint8_t SPI1_ReadWriteByte(uint8_t data);


/* Ði?u khi?n CS */

void BMI160_CS_Low(void);

void BMI160_CS_High(void);


/* Ghi 1 thanh ghi */

void SPI1_WriteRegister(
    uint8_t reg,
    uint8_t data
);


/* Ð?c 1 thanh ghi */

uint8_t SPI1_ReadRegister(
    uint8_t reg
);


/* Ð?c nhi?u byte */

void SPI1_ReadBuffer(
    uint8_t reg,
    uint8_t *buffer,
    uint8_t length
);


#endif