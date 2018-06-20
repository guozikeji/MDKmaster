
#ifndef __BSP232_H
#define __BSP232_H
#include "stm32f10x_conf.h "
#include "common.h"

void BspRs232DmaWrite(u16 length);
void BspRs232Write(u16 length);
void BspRs232NvicInit(void);
void BspRs232DmaInit(void);
void BspRs232Init(void);
void BspRs232Clear(void);
#endif
