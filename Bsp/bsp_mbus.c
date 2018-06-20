#include "bsp_mbus.h"

#define USART1_DR_Base           0x40013804
u16 const USART1_BUFFER_SIZE=512;
u8 USART1_RX_BUFFER[USART1_BUFFER_SIZE];
u8 USART1_TX_BUFFER[USART1_BUFFER_SIZE];

void BspMbusInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_USART1| 
                           RCC_APB2Periph_AFIO, ENABLE);


	//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		//MBUS使能
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		MBUS_CLOSE;
    /* Configure USART1_Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_SetBits(GPIOA, GPIO_Pin_9);
    /* Configure USART1_Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //RS485 READ OR WRITE	
//    GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE); //USART2_TX-->PD5;USART2_RX-->PD6
    //USART工作在异步模式下
	//MBUS默认波特率2400 数据位8位 停止位1 偶校验
		USART_InitStructure.USART_BaudRate = 2400;//波特率
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;//数据位数
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_Even ;//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件控制流
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//发送接收均使能
		/* Configure the USARTx */ 
		USART_Init(USART1, &USART_InitStructure);
		USART_Cmd(USART1, ENABLE); 
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		BspMbusDmaInit();
		BspMbusNvicInit();
		BspMbusTim4Config();
}

void BspMbusDmaInit(void)
{
		DMA_InitTypeDef DMA_InitStructure; 
		/* DMA clock enable */  
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1 
		/* DMA1 Channel4 (triggered by USART1 Tx event) Config */ 
		DMA_DeInit(DMA1_Channel4); 
		DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;  
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_TX_BUFFER; 
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
		DMA_InitStructure.DMA_BufferSize = USART1_BUFFER_SIZE;  
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
		DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
		DMA_Init(DMA1_Channel4, &DMA_InitStructure);  
		/* Enable USART1 DMA TX request */  
		USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); 
		DMA_Cmd(DMA1_Channel4, DISABLE);  

		/* DMA1 Channel5 (triggered by USART1 Rx event) Config */ 

//		DMA_DeInit(DMA1_Channel5); 
//		DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;  
//		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART2_RX_BUFFER; 
//		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
//		DMA_InitStructure.DMA_BufferSize = USART2_BUFFER_SIZE;  
//		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
//		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
//		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
//		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
//		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
//		DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
//		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
//		DMA_Init(DMA1_Channel5, &DMA_InitStructure);  
////		DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE); 
////		DMA_ITConfig(DMA1_Channel5, DMA_IT_TE, ENABLE); 	
//		/* Enable USART1 DMA RX request */  
//		USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); 
//		DMA_Cmd(DMA1_Channel5, ENABLE);  		
}
void BspMbusNvicInit(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
			/* Enable AFIO clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		
		/* Configure PA.00 pin as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOA, &GPIO_InitStructure);		
		/* Connect EXTI0 Line to PA.00 pin */
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);	
		EXTI_ClearITPendingBit(EXTI_Line1);
		EXTI_InitStructure.EXTI_Line = EXTI_Line1;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling ;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);  

		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure);  	
}
//配置PB8 PWM输出 
void BspMbusTim4Config(void)
{	
	/* Remap使能并配置 为开漏输出 为1高电平 断开 为0低电平 接通 */
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO, ENABLE);
	/* Configure pb8 as alternate function (TIM4_OC3) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	/* TIM3 configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 71;   /* TIM4CLK = 1 MHz */
	TIM_TimeBaseStructure.TIM_Period = 100;   // PWM frequency : 22KHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	//	TIM_OCInitStructure.TIM_Channel = TIM_Channel_3;  	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_Pulse = 50;  /* Duty cycle: 50%*/
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_Cmd(TIM4, ENABLE);	
}
void BspMbusWrite(u16 length)
{
	while (DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET)
	{if(DMA_GetFlagStatus(DMA1_FLAG_GL4)==RESET)break;}
	DMA_Cmd(DMA1_Channel4, DISABLE); 
	DMA1_Channel4->CNDTR=length;
	DMA_Cmd(DMA1_Channel4, ENABLE);
}
