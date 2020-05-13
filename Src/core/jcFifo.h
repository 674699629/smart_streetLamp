#ifndef __FIFO_H__
#define __FIFO_H__

//#include "jcUtilsConf.h"
#include "main.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  uint16_t head;
  uint16_t tail;
  uint16_t bufSize;
  uint8_t *fifoBuf;
}FifoDat_Typedef;
FifoDat_Typedef jcFifoInit(uint8_t *buf,uint16_t size);
uint16_t jcFifoUsedLen(FifoDat_Typedef *fifo);
uint16_t jcFifoFreeLen(FifoDat_Typedef *fifo);
void jcFifoClear(FifoDat_Typedef *fifo);
void jcFifoDelete(FifoDat_Typedef *fifo);
uint16_t jcFifoWrite(FifoDat_Typedef *fifo,void *buf,uint16_t len);
int8_t jcFifoCheckOffset(FifoDat_Typedef *fifo,void *buf,uint16_t offset);
uint16_t jcFifoReadByte_noDel(FifoDat_Typedef *fifo,void *buf,uint16_t offset,uint16_t len);
uint16_t jcFifoReadByte_del(FifoDat_Typedef *fifo,void *buf,uint16_t len);
void jcFifoDeleteNByte(FifoDat_Typedef *fifo,uint16_t len);

#ifdef __cplusplus
}
#endif

#endif
