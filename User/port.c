

#include "port.h"
//#include "rs485.h"
#include "common.h"
#include "delay.h"
extern u16 LedTimer ;
extern u8  OutputLedStatus;
extern u8  Start_Flag;
extern u16 DelayTimer;
u8 NoneTapeFlag = 0;
void led_display(void)
{
 if(Sensor_In == RESET){ SensorLed = ON; }else { SensorLed = OFF; }
 
 if(LedTimer < 500) {System_Run = ON;}
 else if(LedTimer < 1000){System_Run = OFF;}
 else LedTimer = 0;
 
}
void Output_display(u8 status)      // Êä³öµÆ 0 green 1 red 2 yellow 3 buzzer
{
 switch(status)
 {
	 case 0x00:
	 GPIOE->BSRR = 0x40000080;
	 GPIOB->BSRR = 0x00000c00;
	 break;
	 
	 case 0x01:
	 GPIOE->BSRR = 0x0000c000;
	 GPIOB->BSRR = 0x08000400; 
	 break;
	 
	 case 0x02:
	 GPIOE->BSRR = 0x80004000;
	 GPIOB->BSRR = 0x00000c00; 
	 break;
	 
	 case 0x03:
	 GPIOE->BSRR = 0x0000c000;
	 GPIOB->BSRR = 0x04000800;  
	 break;
	 default: break;
 }
}

void NoneTape_Func(void)
{
	OutputLedStatus = 3;
	DelayTimer = 0;
	//while(DelayTimer <10){};
  OutputLedStatus = 1;
	Start_Flag = 0;
	NoneTapeFlag = 1;
	
}
