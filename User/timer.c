#include "timer.h"
#include "common.h"
#include "delay.h"
#define TimerPeriod  ((SystemCoreClock / 48 / 1000  ) - 1)//频率=SystemCoreClock/(分频+1)-1
#define StartSpeed	500		//从250hz开始加速
u32  StepCount = 0;
u16 BspTimSpeed=3000; 
u16 BspTimPeriod;
u16 BspTimPulse;	
extern u16 DelayTimer;	
void TIM3_PWM_Init(u16 arr , u16 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,  ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );  //使能GPIO外设和AFIO复用功能模块时钟
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOA.
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = arr/2 ;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
  
	 /*  TIM3 中断嵌套设计*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	TIM3->CCER &= 0xffef;   
	//TIM_Cmd(TIM3,DISABLE);  //使能TIM3
	TIM_Cmd(TIM3,ENABLE); 
}

void StepRun_Func(u8 status,u16 speed, u32 length,u16 *count )
 {
	u16 tmpspeed = 0;
  tmpspeed =  speed * 400;
	if(status == ON && count != 0 && Sensor_In == RESET)
	{
		   Step_Enable = OFF;
       TIM_SetAutoreload(TIM3,tmpspeed); //  改变频率  
	     TIM_SetCompare2(TIM3,tmpspeed/2);   // 改变占空比
	     StepCount = (length * 5.81)*1.2;
	     TIM3->CCER |= 0x0010;
   }
 }
 void StepOnce_Func(u8 status,u16 speed, u32 length)
 {
  u16 tmpspeed = 0;
  tmpspeed =  speed * 400;
	if(status == ON )
	{
	 Step_Enable = OFF;
   TIM_SetAutoreload(TIM3,tmpspeed); //  改变频率  
	 TIM_SetCompare2(TIM3,tmpspeed/2);   // 改变占空比
	 StepCount = (length * 5.81)*1.2;
	 TIM3->CCER |= 0x0010;
	}
 }
 
