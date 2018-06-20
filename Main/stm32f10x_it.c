/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "port.h"
//extern u16 StepCount ;
//extern u32 PUL_CNT;   // TIM3脉冲计数
//extern vu32 step_done;
extern u16 Rs232ReceiveCount;
extern u8 Rs232RxBuffer[];
extern u8 Rs232RxComplete;
extern u32 StepCount;
extern u16 WorkNumber_Value ;
extern u8 Start_Flag; 
extern u8 OnceFlag;
extern u8 Mode;
u32 ServoPlusCheck;
u16 LedTimer;
u16 SendDataTimer;
u16 DelayTimer;
/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  LedTimer++;
	SendDataTimer++;
	DelayTimer++;
}



/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
/**
  * @brief  This function handles CAN1 RX0 Handler.
  * @param  None
  * @retval None
  */

void USB_LP_CAN1_RX0_IRQHandler(void)

{

}
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
  	TIM3->SR=0;
//		StepCount=StepCount+1;
//		TIM_ClearFlag(TIM3,TIM_FLAG_Update);
//		step_done = 1; 

  }
}
 /**
   * @brief  This function handles ADC1 and ADC2 global interrupts requests.
   * @param  None
   * @retval None
   */

void ADC1_2_IRQHandler(void)
{

 }


void WWDG_IRQHandler(void)
{
	//flash 看门狗屏蔽
	WWDG_ClearFlag(); 
//	if(IT_DOG&FLASH_DOG)
//	{
//		WWDG_SetCounter(127);
//		IT_DOG=0;
//	}
	
}
/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
  }
}
	
void USART2_IRQHandler(void)
{
if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{	/* Read one byte from the receive data register */
		Rs232RxBuffer[Rs232ReceiveCount++] =  USART_ReceiveData(USART2);
		switch(Rs232ReceiveCount)
		{
			case 1:  if(Rs232RxBuffer[0] != Start_Code)
               {
							  Rs232ReceiveCount = 0;                    // 判定 第一个数据 是不是 开始码  这里没有 进行长度校验
							 }
							 break;
//			case 2: if(Rs232RxBuffer[1] != 0x43 ||Rs232RxBuffer[1] != 0x44 )
//			         {
//			         Rs232ReceiveCount = 0;
//			         }	
//               break;							 
		 default: break;
		}
		if(Rs232ReceiveCount >= RS232_BUFFER_SIZE)
		{
		 Rs232RxComplete= Failure;
		 Rs232ReceiveCount = Failure;                      // 接收失败
		}
 }
}
/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
//	u16 i;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
//  	 RS485_RX2_BUFFER[RS485_Receive2Count++] = USART_ReceiveData(USART3);		
//		 RS485_Time20=0;
//		switch(RS485_Receive2Count)
//		{
//			case 1:if(RS485_RX2_BUFFER[0]!=START_CODE)RS485_Receive2Count=0;break;
//			default:
//						if(RS485_Receive2Count==13)if(RS485_RX2_BUFFER[12]==END_CODE)RS485_RX2_COMPLETE=1;
//						if(RS485_Receive2Count>=RS485_BUFFER2_SIZE){RS485_RX2_COMPLETE=0;RS485_Receive2Count=0;}
//						break;
//		}			

	}
}

/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void)
{

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
//		/* Read one byte from the receive data register */
//  		Rs485RxBuffer[Rs485ReceiveLength++] = USART_ReceiveData(UART4);		
//			if(Rs485RxComplete==0)Rs485RxComplete=1;RS485_Time1=0;
//			switch(Rs485ReceiveLength)
//			{
//				case 1:if(Rs485RxBuffer[0]!=START_CODE)Rs485ReceiveLength=0;break;
//				default:
//							if(Rs485RxBuffer[Rs485ReceiveLength-1]==END_CODE)Rs485RxComplete=2;
//							if(Rs485RxComplete!=2)
//								{
//									if((Rs485RxBuffer[Rs485ReceiveLength-1]<0x20)|(Rs485RxBuffer[Rs485ReceiveLength-1]>0x7F))
//									Rs485RxComplete=0;RS485_Time1=0;
//								}
//							if(Rs485ReceiveLength>=RS485_BUFFER1_SIZE){Rs485RxComplete=0;Rs485ReceiveLength=0;}
//							break;
//			}		
	}   
}
void DMA1_Channel4_IRQHandler(void)
{
//	DMA_ClearITPendingBit(DMA1_IT_TC4);
//	DMA_ClearITPendingBit(DMA1_IT_TE4);
//	DMA_Cmd(DMA1_Channel4, DISABLE);//关闭DMA,防止处理其间有数据
//	
//	USART_ITConfig(USART1, USART_IT_TC , ENABLE);//开启空闲,帧错,噪声,校验错中断

}

void DMA1_Channel5_IRQHandler(void)
{
//	DMA_ClearITPendingBit(DMA1_IT_TC5);
//
//	DMA_Cmd(DMA1_Channel5, DISABLE);//关闭DMA,防止处理其间有数据
//
//    DMA1_Channel5->CNDTR = RS485_NUM;//重装填
//    DMA_Cmd(DMA1_Channel5, ENABLE);//处理完,重开DMA
//
//	USART_ClearFlag(USART1, USART_FLAG_TC);

}
void TIM4_IRQHandler(void)
{

 	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
//		DMA_ClearFlag(DMA1_FLAG_TC4);      //清DMA发送完成标志
//		DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
//		USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
//		DMA_Cmd(DMA1_Channel4, ENABLE);    //启动DMA
//		TIM_Cmd(TIM4, DISABLE);
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

	}
}
void TIM2_IRQHandler(void)
{

 	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
	
//		SET_RS485_RECV;
//		DMA_Cmd(DMA1_Channel5, ENABLE);    //启动DMA
// 		USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
//		USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);
//		TIM_Cmd(TIM2, DISABLE);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	}
}
/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);

//    GPRS_IdleTimeSecondCounter++;	    //GPRS GPRS 网络空闲时间 分钟
//	GPRS_HeartbeatTimeSecondCounter++;	   //GPRS GPRS 心跳包时间 S
//	if(RTC_TIME_DayOne>=86400)
//	{
//		USER_Time_GetUnixTime();
//		RTC_TIME_DayOne=RTC_TIME[3]*3600+RTC_TIME[4]*60+RTC_TIME[5];
//	}
//	else RTC_TIME_DayOne++;
//	RTC_TIME[3]=RTC_TIME_DayOne/3600;
//	RTC_TIME[4]=(RTC_TIME_DayOne%3600)/60;
//	RTC_TIME[5]=(RTC_TIME_DayOne%3600)%60;
////	// 时间段1-8
//	if((RTC_TIME_DayOne<MBUS_TimeBucketSecond[1])&(RTC_TIME_DayOne>=MBUS_TimeBucketSecond[0]))MBUS_NowTimeBucket=1;
//	else if((MBUS_TimeBucketSecond[2]>=MBUS_TimeBucketSecond[1])&(RTC_TIME_DayOne<MBUS_TimeBucketSecond[2])&((RTC_TIME_DayOne>=MBUS_TimeBucketSecond[1])))MBUS_NowTimeBucket=2;
//	else if((MBUS_TimeBucketSecond[3]>=MBUS_TimeBucketSecond[2])&(RTC_TIME_DayOne<MBUS_TimeBucketSecond[3])&((RTC_TIME_DayOne>=MBUS_TimeBucketSecond[2])))MBUS_NowTimeBucket=3;
//	else if((MBUS_TimeBucketSecond[4]>=MBUS_TimeBucketSecond[3])&(RTC_TIME_DayOne<MBUS_TimeBucketSecond[4])&((RTC_TIME_DayOne>=MBUS_TimeBucketSecond[3])))MBUS_NowTimeBucket=4;
//	else if((MBUS_TimeBucketSecond[5]>=MBUS_TimeBucketSecond[4])&(RTC_TIME_DayOne<MBUS_TimeBucketSecond[5])&((RTC_TIME_DayOne>=MBUS_TimeBucketSecond[4])))MBUS_NowTimeBucket=5;
//	else if((MBUS_TimeBucketSecond[6]>=MBUS_TimeBucketSecond[5])&(RTC_TIME_DayOne<MBUS_TimeBucketSecond[6])&((RTC_TIME_DayOne>=MBUS_TimeBucketSecond[5])))MBUS_NowTimeBucket=6;
//	else if((MBUS_TimeBucketSecond[7]>=MBUS_TimeBucketSecond[6])&(RTC_TIME_DayOne<MBUS_TimeBucketSecond[7])&((RTC_TIME_DayOne>=MBUS_TimeBucketSecond[6])))MBUS_NowTimeBucket=7;
//	else MBUS_NowTimeBucket=8;


  }
}
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
//        Read_ADS(&ADS_X,&ADS_Y);
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
//		GPIO_SetBits(GPIOB, GPIO_Pin_0);
		EXTI_ClearITPendingBit(EXTI_Line0);
	}

}

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
//		GPIO_SetBits(GPIOA, GPIO_Pin_0);
//		MBUS_OpenFlag=0;
		EXTI_ClearITPendingBit(EXTI_Line1);
	}

}
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
     ServoPlusCheck++;
	if(ServoPlusCheck > StepCount)
	{
		 Step_Enable = ON;
	   TIM3->CCER &= 0xFFEF;
     ServoPlusCheck = 0;	
		 OnceFlag = 0;
	 if(Mode == 0)
		{
		  if(Sensor_In != RESET)     // 判断 是否有胶带
		  {
		   if(WorkNumber_Value >0) {WorkNumber_Value --;} 
			 if(WorkNumber_Value == 0) {Start_Flag = OFF;} 
		  }
	 else
		  {
		   NoneTape_Func();
			}
	  }
	 }
	 EXTI_ClearITPendingBit(EXTI_Line4);
	}
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */

/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
