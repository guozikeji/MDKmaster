#include "timer.h"
#include "common.h"
#include "delay.h"
#define TimerPeriod  ((SystemCoreClock / 48 / 1000  ) - 1)//Ƶ��=SystemCoreClock/(��Ƶ+1)-1
#define StartSpeed	500		//��250hz��ʼ����
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
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,  ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOA.
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse = arr/2 ;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
  
	 /*  TIM3 �ж�Ƕ�����*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM3->CCER &= 0xffef;   
	//TIM_Cmd(TIM3,DISABLE);  //ʹ��TIM3
	TIM_Cmd(TIM3,ENABLE); 
}

void StepRun_Func(u8 status,u16 speed, u32 length,u16 *count )
 {
	u16 tmpspeed = 0;
  tmpspeed =  speed * 400;
	if(status == ON && count != 0 && Sensor_In == RESET)
	{
		   Step_Enable = OFF;
       TIM_SetAutoreload(TIM3,tmpspeed); //  �ı�Ƶ��  
	     TIM_SetCompare2(TIM3,tmpspeed/2);   // �ı�ռ�ձ�
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
   TIM_SetAutoreload(TIM3,tmpspeed); //  �ı�Ƶ��  
	 TIM_SetCompare2(TIM3,tmpspeed/2);   // �ı�ռ�ձ�
	 StepCount = (length * 5.81)*1.2;
	 TIM3->CCER |= 0x0010;
	}
 }
 
