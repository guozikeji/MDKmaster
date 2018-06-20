
#ifndef __BSP_USART_H
#define __BSP_USART_H
#include "stm32f10x_conf.h "



void Bsp1Rs232Write(u16 length);

void Bsp1Rs232NvicInit(void);
void Bsp1Rs232Init(void);
void Bsp1Rs232Clear(void);


void Bsp1Rs485Write(u16 length);
void Bsp1Rs485NvicInit(void);

void Bsp1Rs485Init(void);
void Bsp1Rs485Clear(void);

void Bsp2Rs485Write(u16 length);
void Bsp2Rs485Init(void);

void Bsp2Rs485NvicInit(void);
void Bsp2Rs485Clear(void);
//void Bsp2Rs485DmaWrite(u16 length);
#endif
