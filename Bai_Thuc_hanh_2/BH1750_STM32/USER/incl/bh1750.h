#ifndef __BH1750_H
#define __BH1750_H

#include "define.h"
#include "app_i2c.h"
#include "delay.h"

#define BH1750_ADDR       0x23
#define BH1750_ONE_H_MODE 0x20

void BH1750_Init(void);
float BH1750_ReadLux(void);

#endif