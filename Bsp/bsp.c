

#include "bsp.h"
#include "delay.h"

void BspInit(void)
{
	BspClock72SetOut();
  BspRs232Init();
  BspGpioInit();	
  EXITX_Init();
 // �Ҽ���һЩ����
	//delay_init();
}
