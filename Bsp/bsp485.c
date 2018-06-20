
#include "bsp485.h"


u8 Rs485RxBuffer[RS485_BUFFER_SIZE];
u8 Rs485TxBuffer[RS485_BUFFER_SIZE];
u16 Rs485ReceiveCount=0;
u8 Rs485RxComplete=0;
u16 Rs485Time1=0;
u16 BspRs485LedTime;

void BspRs485Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1, ENABLE);
	/* Configure USART1_Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure USART1_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 57600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);  //空闲中断使能
	USART_Cmd(USART1, ENABLE); 
	USART_ClearFlag(USART1, USART_IT_RXNE); // 清发送外城标志
//	BspRs485DmaInit();
	BspRs485NvicInit();
}

void BspRs485DmaInit(void)
{
		DMA_InitTypeDef DMA_InitStructure; 
		/* DMA clock enable */  
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1 
		/* DMA1 Channel4 (triggered by USART1 Tx event) Config */ 
		DMA_DeInit(DMA1_Channel4); 
		DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_BASE;  
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Rs485TxBuffer; 
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
		DMA_InitStructure.DMA_BufferSize = RS485_BUFFER_SIZE;  
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
}
void BspRs485NvicInit(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;  

		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure);  	
}
void BspRs485DmaWrite(u16 length)
{
	while (DMA_GetFlagStatus(DMA1_FLAG_TC7) == RESET)
	{if(DMA_GetFlagStatus(DMA1_FLAG_GL7)==RESET)break;}
	DMA_Cmd(DMA1_Channel7, DISABLE); 
	DMA1_Channel7->CNDTR=length;
	DMA_Cmd(DMA1_Channel7, ENABLE);
}
void BspRs485Write(u16 length)
{
	u16 i;
   USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);	
	for (i=0; i<length; i++)
	{
		while (!(USART1->SR & USART_FLAG_TXE));
		USART_SendData(USART1, Rs485TxBuffer[i]);
	}
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ClearFlag(USART1, USART_FLAG_TC); 

}
void BspRs485Clear(void)
{
	Rs485Time1=0;
	Rs485ReceiveCount=0;
	Rs485RxComplete=0;	
}

