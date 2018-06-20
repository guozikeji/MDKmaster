

#include "bsp_gpio.h"

// 0 设定温度 1 定时上传时间间隔
u16 GlobalWriteData[GLOBAL_BUFFER_SIZE];
u16 GlobalReadData[GLOBAL_BUFFER_SIZE];
extern u8 RS232_TX1_BUFFER[];
u16 GlobalDevice[9];
u8 GlobalStop=0;
void BspGpioInit(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC 
	 					     |RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO ,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	RCC_RTCCLKCmd(ENABLE);
	PWR_BackupAccessCmd(ENABLE);		
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);  

	PWR_BackupAccessCmd(ENABLE); // 允许修改RTC 和后备寄存器 
	RCC_LSEConfig(RCC_LSE_OFF); //关闭外部低速外部时钟信号功能 后，PC13 PC14 PC15 才可以当普通IO用。
	BKP_TamperPinCmd(DISABLE);  //关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用
	PWR_BackupAccessCmd(DISABLE);  //禁止修改后备寄存器
// 配置低速IO
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
// 定义配置 开关量 输入点
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
// 定义 外部输入点
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIOE->BSRR=0xc000;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_15|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIOB->BSRR=0x2c00;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
u8 BspDialSwitch(void)
{
	return(GPIO_ReadInputData(GPIOC)&0x000f);	
}
void BspGlobalInit(void)
{
//	u32 device[3];
	GlobalDevice[2] = *(u32*)(0x1FFFF7E8); 
  GlobalDevice[3]= *(u32*)(0x1FFFF7EA); 
  GlobalDevice[4] = *(u32*)(0x1FFFF7EC); 
	GlobalDevice[5] = *(u32*)(0x1FFFF7EE); 
  GlobalDevice[6]= *(u32*)(0x1FFFF7F0); 
  GlobalDevice[7] = *(u32*)(0x1FFFF7F2); 	
//	device[0] = *(u32*)(0x1FFFF7E8); 
//  device[1]= *(u32*)(0x1FFFF7EC); 
//  device[2] = *(u32*)(0x1FFFF7F0);  
	if(GLOBAL_BUFFER_CODE!=GlobalReadData[GLOBAL_BUFFER_SIZE-1])
	{
		BspGlobalWriteInit();
		BspGlobalWrite();		
	}else if((GlobalWriteData[GLOBAL_BUFFER_SIZE-2]!=GlobalDevice[2])||(GlobalWriteData[GLOBAL_BUFFER_SIZE-3]!=GlobalDevice[3])
				 ||(GlobalWriteData[GLOBAL_BUFFER_SIZE-4]!=GlobalDevice[4])||(GlobalWriteData[GLOBAL_BUFFER_SIZE-5]!=GlobalDevice[5])
				 ||(GlobalWriteData[GLOBAL_BUFFER_SIZE-6]!=GlobalDevice[6])||(GlobalWriteData[GLOBAL_BUFFER_SIZE-7]!=GlobalDevice[7]))
	{
		BspGlobalWriteInit();
		BspGlobalWrite();			
	}else if(GlobalWriteData[GLOBAL_BUFFER_SIZE-9]!=GlobalWriteData[GLOBAL_BUFFER_SIZE-8])			 
	{	
		if(GlobalWriteData[GLOBAL_BUFFER_SIZE-10]<GlobalWriteData[GLOBAL_BUFFER_SIZE-8])	
		{
			GlobalWriteData[GLOBAL_BUFFER_SIZE-10]++;
			BspGlobalWrite();
		}else GlobalStop=1;		
	}	
}
void BspGlobalWriteInit(void)
{
		GlobalWriteData[0]=0X0163; //FAN TEMP35.5
		GlobalWriteData[1]=0X2710; //UP DATA TIME--100.0MS
		GlobalWriteData[2]=0X0004; //  RS232 RESEND COUNT SET
		GlobalWriteData[3]=0X7530; //COMPLETE BEEP TIME 3000.0 MS
		GlobalWriteData[4]=0X7530; //NC BEEP TIME  3000.0 MS
		GlobalWriteData[5]=0XEA60; //FAN TIME  6000.0 MS
		GlobalWriteData[8]=0;  
		GlobalWriteData[9]=0;		
//		GlobalWriteData[GLOBAL_BUFFER_SIZE-10]=0;	                //开机计数
		GlobalWriteData[GLOBAL_BUFFER_SIZE-9]=0;	                //屏蔽计数
		GlobalWriteData[GLOBAL_BUFFER_SIZE-8]=GLOBAL_BUFFER_END;	 //停止计数
		GlobalWriteData[GLOBAL_BUFFER_SIZE-7]=GlobalDevice[7];	
		GlobalWriteData[GLOBAL_BUFFER_SIZE-6]=GlobalDevice[6];	
		GlobalWriteData[GLOBAL_BUFFER_SIZE-5]=GlobalDevice[5];	
		GlobalWriteData[GLOBAL_BUFFER_SIZE-4]=GlobalDevice[4];	
		GlobalWriteData[GLOBAL_BUFFER_SIZE-3]=GlobalDevice[3];	
		GlobalWriteData[GLOBAL_BUFFER_SIZE-2]=GlobalDevice[2];	
		GlobalWriteData[GLOBAL_BUFFER_SIZE-1]=GLOBAL_BUFFER_CODE;	
}
void BspGlobalWrite(void)
{
	FLASH_Status FLASHStatus = FLASH_COMPLETE;	
	u32 address = START_ADDR;
	u16 i;
	FLASH_Unlock();
		/* Clear All pending flags */
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);		
	/* Erase the FLASH pages */
	FLASH_ErasePage(START_ADDR );	
	/*  FLASH Word program of data 0x15041979 at addresses defined by StartAddr and EndAddr*/
	i=0;
	while((address < (START_ADDR+GLOBAL_BUFFER_SIZE*2)) && (FLASHStatus == FLASH_COMPLETE))
	{
		FLASHStatus = FLASH_ProgramHalfWord(address, GlobalWriteData[i]);
		GlobalReadData[i]=GlobalWriteData[i];
		address += 2;i++;
	}
	FLASH_Lock();    //锁定FLASH编程擦除控制器
}
void BspGlobalRead(void)
{
	u32 address = START_ADDR;
	u16 i=0;
	while(address < (START_ADDR+GLOBAL_BUFFER_SIZE*2))
	{
		GlobalReadData[i]=(*(u16*) address);	
		GlobalWriteData[i]=GlobalReadData[i];
		address += 2;i++;
	}
	address = START_ADDR;
	if(GLOBAL_BUFFER_CODE!=GlobalReadData[GLOBAL_BUFFER_SIZE-1])while(address < (START_ADDR+GLOBAL_BUFFER_SIZE*2))
	{
		GlobalReadData[i]=(*(u16*) address);	
		GlobalWriteData[i]=GlobalReadData[i];
		address += 2;i++;
	}
	address = START_ADDR;
	if(GLOBAL_BUFFER_CODE!=GlobalReadData[GLOBAL_BUFFER_SIZE-1])while(address < (START_ADDR+GLOBAL_BUFFER_SIZE*2))
	{
		GlobalReadData[i]=(*(u16*) address);
		GlobalWriteData[i]=GlobalReadData[i];
		address += 2;i++;
	}
	for(i=0;i<GLOBAL_BUFFER_SIZE;i++)GlobalWriteData[i]=GlobalReadData[i];
	
}
