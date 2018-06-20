
#ifndef __RS232_H
#define __RS232_H
#include "stm32f10x_conf.h "
#include "bsp232.h"
#include "common.h"
#include "port.h"

void Rx232ProcessFunc(void);
u8  ConfigProcessFunc(u8 tmp);
void Rs232SendData(void);
u8  OnceWorkingProcessFunc(u8 tmp);
void Rs232SendZeroData(void);
void Rs232SendNoneData(void);
void Rs232SendData_Run_Func(u16 work_number);
#endif
