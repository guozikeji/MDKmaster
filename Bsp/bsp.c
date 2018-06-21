

#include "bsp.h"
#include "delay.h"

void BspInit(void)
{
	BspClock72SetOut();
  BspRs232Init();
  BspGpioInit();	
  EXITX_Init();
 // 我加了一些内容
	//delay_init();
}
