

#include "bsp.h"
#include "delay.h"

void BspInit(void)
{
	BspClock72SetOut();
  BspRs232Init();
  BspGpioInit();	
  EXITX_Init();
	//delay_init();
}
