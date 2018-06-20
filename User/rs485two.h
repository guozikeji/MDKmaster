
#ifndef __RS485TWO_H
#define __RS485TWO_H
#include "stm32f10x_conf.h "
#include "bsp_usart.h"
#include "common.h"

u8 Rs485TwoReceive(void);
void Rs485TwoSendLoop(u8 command);
void Rs485TwoSend(u8 command);
#endif
