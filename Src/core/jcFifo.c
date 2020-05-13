/*************************************************************************
**		(C) COPYRIGHT 2011~  吴健超(DGUT)
**		    源码未经检录,使用需谨慎,修改请注明原作者
** 创建人 @author  : 吴健超
** 版本   @version : V 1.0.0 原始版本
** 日期   @date    : 2016.1.20
** 功能   @brief   : 实现FIFO环的读写与恢复指定长度
** 硬件   @hardware：任何ANSI-C平台
** 其他   @other   ：
***************************************************************************/
#include "jcFifo.h"

/**************************************************************************
** 功能	@brief : 初始化FIFO环的大小，必须在使用FIFO环之前调用
** 输入	@param : buf：fifo缓冲区指针，size：FIFO环的大小
** 输出	@retval:
***************************************************************************/
FifoDat_Typedef jcFifoInit(uint8_t *buf,uint16_t size)
{
    FifoDat_Typedef fifo;
    fifo.head = 0;
    fifo.tail = 0;
    fifo.bufSize = size;
    fifo.fifoBuf =buf;
    return fifo;
}
/**************************************************************************
** 功能	@brief : 计算FIFO环中已用空间大小
** 输入	@param :
** 输出	@retval:
***************************************************************************/
uint16_t jcFifoUsedLen(FifoDat_Typedef *fifo)
{
    uint16_t head = fifo->head;
    uint16_t tail = fifo->tail;
	if(fifo==NULL)
	{
		return fifo->bufSize;
	}
    return (head <= tail)?(tail - head):(fifo->bufSize - head + tail);
}
/**************************************************************************
** 功能	@brief : 计算FIFO环中剩余空间大小
** 输入	@param :
** 输出	@retval:
***************************************************************************/
uint16_t jcFifoFreeLen(FifoDat_Typedef *fifo)
{
    uint16_t head = fifo->head;
    uint16_t tail = fifo->tail;
	if(fifo==NULL)
	{
		return 0;
	}
    return (head > tail)?(head - tail):(fifo->bufSize - tail + head);
}

/**************************************************************************
** 功能	@brief : 清空缓冲区
** 输入	@param :
** 输出	@retval: 
***************************************************************************/
void jcFifoClear(FifoDat_Typedef *fifo)
{
    fifo->head = fifo->tail;
}
/**************************************************************************
** 功能	@brief : 删除缓冲区
** 输入	@param :
** 输出	@retval: 
***************************************************************************/
void jcFifoDelete(FifoDat_Typedef *fifo)
{
    fifo->head = 0;
    fifo->tail = 0;
    fifo->bufSize = 0;
    fifo->fifoBuf = NULL;
}
/**************************************************************************
** 功能	@brief : 向指定FIFO环中写入指定长度数据，若FIFO环未用空间不足，则写满为止
** 输入	@param :
** 输出	@retval: 返回成功写入的长度
***************************************************************************/
uint16_t jcFifoWrite(FifoDat_Typedef *fifo,void *buf,uint16_t len)
{
    uint16_t i,freeLen;
    uint8_t *data = (uint8_t*)buf;
	if(fifo==NULL || fifo->fifoBuf == NULL)
	{
		return 0;
	}
    freeLen = jcFifoFreeLen(fifo);        // fifo环中未用大小
    len = (freeLen>len)?len:freeLen;
    for(i=0;i<len;i++)
    {
        fifo->fifoBuf[fifo->tail++]=*data++;
        if(fifo->tail == fifo->bufSize)
          fifo->tail = 0;
    }
    return len;
}
/**************************************************************************
** 功能	@brief : 读取从FIFO环的头部偏移offset的一个数据,读取后不会删除头部
** 输入	@param : offset:偏移量
** 输出	@retval: 返回成功与否
***************************************************************************/
int8_t jcFifoCheckOffset(FifoDat_Typedef *fifo,void *buf,uint16_t offset)
{
    uint16_t index;
    uint8_t *data = (uint8_t*)buf;
	if(fifo==NULL || fifo->fifoBuf == NULL ||  buf==NULL)
	{
		return -1;
	}
    if(jcFifoUsedLen(fifo)<=offset) // fifo环中有效长度不足
        return -1;

    if(fifo->bufSize - fifo->head >offset)
    {
        index = fifo->head+offset;
    }
    else
    {
        index = offset-(fifo->bufSize-fifo->head);
    }
    *data = fifo->fifoBuf[index];
    return 0;
}
/**************************************************************************
** 功能	@brief : 读取从FIFO环的头部偏移offset开始的len个数据,不会会删除读取后的数据
** 输入	@param : offset:偏移量
** 输出	@retval: 返回成功与否
***************************************************************************/
uint16_t jcFifoReadByte_noDel(FifoDat_Typedef *fifo,void *buf,uint16_t offset,uint16_t len)
{
	uint16_t index;
    uint16_t usedLen = jcFifoUsedLen(fifo);
    uint8_t *data = (uint8_t*)buf;
	if(fifo==NULL || fifo->fifoBuf == NULL ||  buf==NULL || 0 == usedLen || 0 == len)
	{
		return 0;
	}
    len = usedLen < len?usedLen:len;
	for(index = 0;index<len;index++)
	{
		if(fifo->bufSize - fifo->head > offset +index)
		{
			data[index] = fifo->fifoBuf[fifo->head + offset +index];
		}
		else
		{
			data[index] = fifo->fifoBuf[offset+index -(fifo->bufSize - fifo->head)];
		}
	}
	return len;
}
/**************************************************************************
** 功能	@brief : 读取从FIFO环的头部len个数据,会删除读取后的数据
** 输入	@param :
** 输出	@retval: 返回成功与否
***************************************************************************/
uint16_t jcFifoReadByte_del(FifoDat_Typedef *fifo,void *buf,uint16_t len)
{
    uint16_t index;
    uint16_t usedLen = jcFifoUsedLen(fifo);
    uint8_t *data = (uint8_t*)buf;
	if(fifo==NULL || fifo->fifoBuf == NULL ||  buf==NULL || 0 == usedLen || 0 == len)
	{
		return 0;
	}
    len = usedLen < len?usedLen:len;
    for(index = 0;index<len;index++)
    {
         data[index] = fifo->fifoBuf[fifo->head++];
         if(fifo->head == fifo->bufSize)
           fifo->head = 0;
    }
    return len;
}
/**************************************************************************
** 功能	@brief : 删除FIFO首部的N个数据
** 输入	@param :
** 输出	@retval: NULL
***************************************************************************/
void jcFifoDeleteNByte(FifoDat_Typedef *fifo,uint16_t len)
{
	if(fifo==NULL || fifo->fifoBuf == NULL)
	{
		return;
	}
    if(jcFifoUsedLen(fifo)<len)
    {
        fifo->head = fifo->tail;
    }
    else
    {
      fifo->head = (fifo->bufSize-fifo->head >len)?(fifo->head+len):(len-(fifo->bufSize-fifo->head));
    }
}

