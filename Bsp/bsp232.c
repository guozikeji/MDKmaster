
#include "bsp232.h"
#include "common.h"

u8 Rs232RxBuffer[RS232_BUFFER_SIZE];
u8 Rs232TxBuffer[RS232_BUFFER_SIZE];
u16 Rs232ReceiveCount=0;
u8 Rs232RxComplete=0;
u16 Rs232Time1=0;

u16 BspRs232LedTime;
extern u8 SiteBuffer[16];
void BspRs232Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/* Configure USART1_Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure USART1_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 57600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE , ENABLE);  //空闲中断使能
	USART_Cmd(USART2, ENABLE); 
	USART_ClearFlag(USART2, USART_IT_RXNE); // 清发送外城标志
  BspRs232NvicInit();
}

void BspRs232NvicInit(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;  

		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure);  	
}
void BspRs232Write(u16 length)
{
	u16 i;
  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);	
	for (i=0; i<length; i++)
	{
		while (!(USART2->SR & USART_FLAG_TXE));
		USART_SendData(USART2, Rs232TxBuffer[i]);
	}
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ClearFlag(USART2, USART_FLAG_TC); 
	
}
//void BspRs232DmaWrite(u16 length)
//{
//	while (DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET)
//	{if(DMA_GetFlagStatus(DMA1_FLAG_GL4)==RESET)break;}
//	DMA_Cmd(DMA1_Channel4, DISABLE); 
//	DMA1_Channel4->CNDTR=length;
//	DMA_Cmd(DMA1_Channel4, ENABLE);
//}
void BspRs232Clear(void)
{
	Rs232Time1=0;
	Rs232ReceiveCount=0;
	Rs232RxComplete=0;	
}


