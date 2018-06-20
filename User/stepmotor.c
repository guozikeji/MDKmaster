#include "stepmotor.h"
#include "stdio.h"
#include "math.h"
u32 PUL_CNT;   // TIM3�������
vu32 step_done; // 
vu32 run_state; //
extern u8 flag ;
#define run_state_stop 0
#define run_state_acc 1
#define run_state_run 2
#define run_state_dec 3
u32 Time = 0;
u32 Time_Set =0;
void STEPMOTOR_CTRL_INIT(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 NVIC_InitTypeDef NVIC_InitStructure;
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
 TIM_OCInitTypeDef TIM_OCInitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //GPIOʱ��ʹ��
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //��ʱ��3ʱ��ʹ�� 
 //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //��ʱ��2ʱ��ʹ��
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;    //PA7ΪTIM3ͨ��2
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;  //�����������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //GPIO����Ӧ�ٶ� 
 GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;    //PA6ΪDIR�������
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //GPIO����Ӧ�ٶ� 
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	
	//TIM3_Configuration
 TIM_TimeBaseStructure.TIM_Period            = 23999;     //�Զ���װ�ؼĴ���
 TIM_TimeBaseStructure.TIM_Prescaler         = 2;      //Ԥ��Ƶ��,t=(23999+1)*(2+1)/72M
 TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up; //���������ϼ���ģʽ     
 TIM_TimeBaseStructure.TIM_ClockDivision     = 0x0;      //ʱ�ӷ�Ƶ����
 TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;      //ÿ������������¼�����
 TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);       //дTIM3���Ĵ�������
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 TIM_ClearFlag(TIM3,TIM_FLAG_Update);          //�жϱ�־λ����
 TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);       //������/�Ƚ�3�ж�
 TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;   //PWMģʽ2 TIM3_CCMR1[14:12]=111 �����ϼ���ʱ��һ��TIMx_CNT
 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //����/����2�������
 TIM_OCInitStructure.TIM_Pulse       = 11000;       //ȷ��ռ�ձȣ����ֵ��������Ч��ƽ��ʱ�䡣
 TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;      //�������,�͵�ƽ��Ч   
 TIM_OC2Init(TIM3, &TIM_OCInitStructure);         //���ö�ʱ�����ģʽ,�Ƚϲ�����
 TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);       //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

 
 //TIM2_Configuration ////////////////////////////////////////////////////////////////
// TIM_DeInit(TIM2);          //TIM2��������Ϊȱʡֵ��Ĭ��״̬
// TIM_TimeBaseStructure.TIM_Period        = 359;     //�Զ���װ�ؼĴ���
// TIM_TimeBaseStructure.TIM_Prescaler     = 199;     //ʱ��Ԥ��Ƶ��
// TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //���������ϼ���ģʽ          
// TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;         //ʱ�ӷ�Ƶ���� 
// TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);     //����TIM2�Ĵ���������
// TIM_ClearFlag(TIM2,TIM_FLAG_Update);          //�жϱ�־λ����
// TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);       //������/�Ƚ�2�ж�
 //////////////////////////////////////////////////////////////////////////////////////
 NVIC_InitStructure.NVIC_IRQChannel          = TIM3_IRQn ;  //ѡ��ʱ��TIM3
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;          //ѡ������ʽ���ȼ������ж�Ƕ�׼����йأ�
 NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;       //ѡ�������ȼ���ͬ����ʽ���ȼ�����Ӧ˳��
 NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;    //ѡ��ʹ���ж�Դ
 NVIC_Init(&NVIC_InitStructure);
 NVIC_InitStructure.NVIC_IRQChannel          = TIM2_IRQn ;  //ѡ��ʱ��TIM2
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;          //ѡ������ʽ���ȼ������ж�Ƕ�׼����йأ�
 NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;       //ѡ�������ȼ���ͬ����ʽ���ȼ�����Ӧ˳��
 NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;    //ѡ��ʹ���ж�Դ
 NVIC_Init(&NVIC_InitStructure);
}
void TIM3_Configuration(u32 period)
{
// TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 //TIM_OCInitTypeDef TIM_OCInitStructure;
	
 TIM3->ARR = period-1;
 TIM3->CCR2 = period >>2;
// TIM3->PSC = 7;
 //TIM_TimeBaseStructure.TIM_Period            = period-1;      //�Զ���װ�ؼĴ���
 //TIM_TimeBaseStructure.TIM_Prescaler         = 29;    //Ԥ��Ƶ��,f=72M/[(period+1)*(29+1)], ft = 2400000
 //TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up; //���������ϼ���ģʽ     
 //TIM_TimeBaseStructure.TIM_ClockDivision     = 0x0;      //ʱ�ӷ�Ƶ����
 //TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;      //ÿ������������¼�����
// TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);       //дTIM3���Ĵ�������
 //TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;   //PWMģʽ2 TIM3_CCMR1[14:12]=111 �����ϼ���ʱ��һ��TIMx_CNT
// TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //����/����2�������
 //TIM_OCInitStructure.TIM_Pulse       = period >> 2;     //ȷ��ռ�ձȣ�25%
 //TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;      //�������,�͵�ƽ��Ч   
 //TIM_OC2Init(TIM3, &TIM_OCInitStructure);         //���ö�ʱ�����ģʽ,�Ƚϲ�����
}




//void MOTOR_RUN(u32 acc, u32 dec, u32 topspeed, u32 dis)
//����������в���
//acc -- ���ٶ�,��λ: round/min/s
//dec -- ���ٶ�,��λ: round/min/s
//topspeed -- ����ٶ�,��λ: round/min
//dis -- �ܽ�λ��,��λ: round/10000
void MOTOR_RUN(u32 acc, u32 dec, u32 topspeed, u32 dis)
{
  u32 t_acc,t_dec,step_all,step_acc,step_dec,step_run;
  u32 i,tim_cnt,tim_rest,tim_cnt_temp;
  
  step_all = (float)dis * (N_MOTOR * 0.0001);
  t_acc = topspeed * 1000 / acc; //unit: ms
  t_dec = topspeed * 1000 / dec; //unit: ms
  if(topspeed * (t_acc + t_dec) / 12 > dis)  //�ﲻ������ٶ� // topspeed/60/1000 * (t_acc + t_dec) / 2 > dis / 10000
  {
    topspeed = sqrt(dis * acc * dec * 12 / (acc + dec) / 1000);
    t_acc = topspeed * 1000 / acc; //unit: ms
    t_dec = topspeed * 1000 / dec; //unit: ms 
  }
  step_acc = N_MOTOR * ((float)topspeed*topspeed/(acc*120));
  step_dec = N_MOTOR * ((float)topspeed*topspeed/(dec*120));
  if(step_all > step_acc + step_dec)
   step_run = step_all - step_acc - step_dec;
  else
   step_run = 0;
  //tim_cnt = 5.2363 * ft / (sqrt(acc*N_MOTOR/2)); //(ft * sqrt(60)*0.676) / sqrt(acc*N_MOTOR/2);
  tim_cnt = 7.7460 * ft / (sqrt(acc*N_MOTOR/2));
	Time = tim_cnt;
  tim_rest = 0;
  i = 0;
  TIM3_Configuration(tim_cnt);
  run_state = run_state_acc;
  TIM_Cmd(TIM3,ENABLE);
  step_done = 0;
  while(step_done==0);
  while(i < step_acc)
  {
    i++;
    //tim_cnt_temp = tim_cnt;
    //tim_cnt = tim_cnt - (2*tim_cnt+tim_rest) / (4*i+1);
    //tim_rest = (2*tim_cnt_temp+tim_rest) % (4*i+1);
    tim_cnt_temp = tim_cnt / ( sqrt((float)(i+1)) + sqrt((float)(i)) );
	  Time_Set = tim_cnt_temp;
    TIM3_Configuration(tim_cnt_temp);
		 step_done = 0;
    while(step_done==0);
  }
  
  if(step_run > 0)
  {
    run_state = run_state_run;
    tim_cnt = ft * 60 / (N_MOTOR*topspeed);
		Time = tim_cnt;
    i = 0;
    TIM3_Configuration(tim_cnt);
    while(i < step_run )
    {
      step_done = 0;
      while(step_done==0);
      i++;
    }
  }
  run_state = run_state_dec;
  tim_rest = 0;
  i=0;
  tim_cnt = tim_cnt + (2*tim_cnt+tim_rest) / (4*(step_dec-i)-1);
	Time = tim_cnt;
  while(i <  step_dec)
  {
    TIM3_Configuration(tim_cnt);
    step_done = 0;
    while(step_done==0);
    i++;
    tim_cnt_temp = tim_cnt;
    tim_cnt = tim_cnt + (2*tim_cnt+tim_rest) / (4*(step_dec-i)-1);
    tim_rest = (2*tim_cnt_temp+tim_rest) % (4*(step_dec-i)-1);
  }
  run_state = run_state_stop;
  TIM_Cmd(TIM3,DISABLE);
	flag =1;

}
//void TIM2_IRQHandler(void)
//{
//}
//void TIM3_IRQHandler(void)
//{
//  TIM_ClearFlag(TIM3,TIM_FLAG_Update);
//  step_done = 1; 
// //PUL_CNT++;  
//}

