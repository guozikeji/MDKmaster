#ifndef  _TIMER_H
#define  _TIMER_H
#include "stm32f10x.h"
#define TimerPeriod  ((SystemCoreClock / 48 / 1000  ) - 1)//ÆµÂÊ=SystemCoreClock/(·ÖÆµ+1)-1

void TIM3_PWM_Init(u16 arr, u16 psc); 
//void f(u16 Vt,u16 a,u16 d,u16 S);
void StepRun_Func(u8 status,u16 speed, u32 length,u16 *count);
void StepOnce_Func(u8 status,u16 speed, u32 length);
#endif



