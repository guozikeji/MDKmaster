#ifndef _STEPMOTOR_H
#define _STEPMOTOR_H
 #include "stm32f10x.h"

#define N_MOTOR 1000//步进电机细分
#define ft 24000000
void STEPMOTOR_CTRL_INIT(void);
void MOTOR_RUN(u32 acc, u32 dec, u32 topspeed, u32 dis);

#endif
