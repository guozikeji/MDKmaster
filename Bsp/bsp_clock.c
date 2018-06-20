

#include "bsp_clock.h"

/* 72MHZ 系统内部时钟 */
void BspClock72SetOut(void)
{
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

 // RCC_HSICmd(ENABLE);
	RCC_HSEConfig(RCC_HSE_ON); 	
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
	{
	}
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
 
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);
			 /* ADCCLK = PCLK2/8  9MHZ*/
     RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
	if (SysTick_Config(SystemCoreClock / 1000))
     { 
      /* Capture error */ 
      while (1);
     }

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

void BspWwdgConfiguration(void)
{
	/* WWDG configuration --------------------------------------------------------*/
  	/* Enable WWDG clock */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE); // ??APB1??WWDG?? ;

  	/* WWDG clock counter = (PCLK1/4096)/8 = 1098 Hz (~1 ms)  */
  	WWDG_SetPrescaler(WWDG_Prescaler_8); // ??WWDG??????(PCLK/4096)/8 ;

  	/* Set Window value to 65 */
  	WWDG_SetWindowValue(0X43); // ??WWDG????65 ;

  	/* Enable WWDG and set counter value to 127, WWDG timeout = ~4 ms * 64 = 262 ms */
  	WWDG_Enable(0X7F); // ??WWDG????????127 ;

  	/* Clear EWI flag */
  	WWDG_ClearFlag(); // ??????????? ;

  	/* Enable EW interrupt */
  	WWDG_EnableIT(); // ??WWDG??????(EWI);
 }
void BspIwdgConfiguration(void)
{
		RCC_LSICmd(ENABLE);//
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);      /* ??0x5555,????????????? */
    IWDG_SetPrescaler(IWDG_Prescaler_256);      /* ??????,256/40K=1250HZ(0.8ms)*/
    IWDG_SetReload(1000);        /* ???? 100ms/0.8MS=125 .??????0xfff*/
    IWDG_ReloadCounter();	  //??
    IWDG_Enable();
}

