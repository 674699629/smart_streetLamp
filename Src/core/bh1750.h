#ifndef __BH1750_H
#define __BH1750_H
#include "stm32f0xx.h"


extern __IO uint16_t AHT_TimeCnt;

void BH1750_Init(void);
void BH1750_GetData(void);


#endif
