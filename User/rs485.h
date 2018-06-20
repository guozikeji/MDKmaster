
#ifndef __RS485_H
#define __RS485_H
#include "stm32f10x_conf.h "
#include "bsp485.h"
#include "port.h"
#include "common.h"

void Rs485DataInit(void);
void Rs485_Get_Site(void);
void Rs485SendLoop(u8 status);
void Rs485SendData(u8 status);
void Rs485SendZeroData(u8 site);
void Rs485_LoopSend_Config_func(void);
void Rs485SendProcessData(u8 Status,u8 number,u8 SiteNumber);
#endif
