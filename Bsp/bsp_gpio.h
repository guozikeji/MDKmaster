
#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H
#include "stm32f10x_conf.h "

#define GLOBAL_BUFFER_SIZE   27
#define START_ADDR           ((u32)0x0800F000)

//5A AA 02 00 03 09 16
#define GLOBAL_BUFFER_CODE    0X123A
#define GLOBAL_BUFFER_END      3

u8 BspDialSwitch(void);
void BspGpioInit(void);
void BspGpioLedOff(u8 led);
void BspGpioLedOn(u8 led);

void BspGlobalWriteInit(void);
void BspGlobalRead(void);
void BspGlobalWrite(void);
void BspGlobalInit(void);
#endif
