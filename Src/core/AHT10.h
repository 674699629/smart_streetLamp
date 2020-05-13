#ifndef __AHT10_H
#define __AHT10_H
#include "stm32f0xx.h"


extern __IO uint16_t AHT_TimeCnt;
extern float Humidity,Temperature;



extern void AHT_Init(void);
extern void AHT_Reset(void);
extern void AHT_GetData(void);


#endif
