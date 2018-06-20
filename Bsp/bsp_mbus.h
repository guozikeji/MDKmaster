
#ifndef _BSP_MBUS_H
#define _BSP_MBUS_H
#include "stm32f10x_conf.h "

#define MBUS_SHORT_OUT_OUTPUT  GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0)
#define MBUS_SHORT_OUT_INPUT  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)
#define MBUS_OPEN  GPIO_ResetBits(GPIOA, GPIO_Pin_0)
#define MBUS_CLOSE  GPIO_SetBits(GPIOA, GPIO_Pin_0)

void BspMbusInit(void);
void BspMbusDmaInit(void);
void BspMbusNvicInit(void);
void BspMbusTim4Config(void);
void BspMbusWrite(u16 length);
#endif /* __TIM_H */

