
#include "bsp_usart.h"

//#define USART1_DR_Base           0x40013804
u16 const RS232_BUFFER1_SIZE=512;
u8 RS232_RX1_BUFFER[RS232_BUFFER1_SIZE];
u8 RS232_TX1_BUFFER[RS232_BUFFER1_SIZE];
u16 RS232_Receive1Count=0;
u8 RS232_RX1_COMPLETE=0;
u16 RS232_Time1=0;

//#define USART2_DR_Base           0x40004404
u16 const RS485_BUFFER1_SIZE=512;
u8 RS485_RX1_BUFFER[RS485_BUFFER1_SIZE];
u8 RS485_TX1_BUFFER[RS485_BUFFER1_SIZE];
u16 RS485_Receive1Count=0;
u8 RS485_RX1_COMPLETE=0;
u16 RS485_Time10=0,RS485_Time11=0;

//#define USART3_DR_Base           0x40004804
u16 const RS485_BUFFER2_SIZE=512;
u8 RS485_RX2_BUFFER[RS485_BUFFER2_SIZE];
u8 RS485_TX2_BUFFER[RS485_BUFFER2_SIZE];
u16 RS485_Receive2Count=0;
u8 RS485_RX2_COMPLETE=0;
u16 RS485_Time20=0,RS485_Time21=0;

void Bsp1Rs232Init(void)
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_BaudRate = 57600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE , ENABLE);  //ø’œ–÷–∂œ πƒ‹
	USART_Cmd(USART2, ENABLE); 
	USART_ClearFlag(USART2, USART_IT_RXNE); // «Â∑¢ÀÕÕ‚≥«±Í÷æ
//	Bsp1Rs232DmaInit();
	Bsp1Rs232NvicInit();
}

void Bsp1Rs232NvicInit(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;  

		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure);  	
}
void Bsp1Rs232Write(u16 length)
{
	u16 i;
	for (i=0; i<length; i++)
	{
		while (!(USART2->SR & USART_FLAG_TXE));
		USART_SendData(USART2, RS232_TX1_BUFFER[i]);
	}
}

void Bsp1Rs232Clear(void)
{
	RS232_Time1=0;
	RS232_Receive1Count=0;
	RS232_RX1_COMPLETE=0;	
}

void Bsp1Rs485Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_USART1|
												 RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure USART1_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);  //ø’œ–÷–∂œ πƒ‹
	USART_Cmd(USART1, ENABLE); 
	USART_ClearFlag(USART1, USART_IT_RXNE); // «Â∑¢ÀÕÕ‚≥«±Í÷æ
	  Bsp1Rs485NvicInit();
}

void Bsp1Rs485NvicInit(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;  

		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure);  	
}

void Bsp1Rs485Write(u16 length)
{
	u16 i;
	for (i=0; i<length; i++)
	{
		while (!(USART1->SR & USART_FLAG_TXE));
		USART_SendData(USART1, RS485_TX1_BUFFER[i]);
	}
}

void Bsp1Rs485Clear(void)
{
	RS485_Time10=0;
	RS485_Receive1Count=0;
	RS485_RX1_COMPLETE=0;	
}
void Bsp2Rs485Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	/* Configure USART1_Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* Configure USART1_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE , ENABLE);  //‡†ï–ê◊ê◊è π≈ú
	USART_Cmd(USART3, ENABLE); 
	USART_ClearFlag(USART3, USART_IT_RXNE); // »•◊¢ÃçŒ¢‘á“™÷æ
	USART_ClearFlag(USART3, USART_FLAG_TXE); 

//   	USART_HalfDuplexCmd(UART4, ENABLE);
//	  Bsp2Rs485DmaInit();
	  Bsp2Rs485NvicInit();
}

void Bsp2Rs485NvicInit(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;  

		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure);  	
}

void Bsp2Rs485Write(u16 length)
{
	u16 i;

	for (i=0; i<length; i++)
	{
		while (!(USART3->SR & USART_FLAG_TXE));
		USART_SendData(USART3, RS485_TX2_BUFFER[i]);
	} 	
}

void Bsp2Rs485Clear(void)
{
	RS485_Time20=0;
	RS485_Receive2Count=0;
	RS485_RX2_COMPLETE=0;	
}

















