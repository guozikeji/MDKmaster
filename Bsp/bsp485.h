
#ifndef __BSP485_H
#define __BSP485_H
#include "stm32f10x_conf.h "
#include "common.h"

void BspRs485DmaWrite(u16 length);
void BspRs485Write(u16 length);
void BspRs485DmaInit(void);
void BspRs485NvicInit(void);
void BspRs485Init(void);
void BspRs485Clear(void);

#endif
