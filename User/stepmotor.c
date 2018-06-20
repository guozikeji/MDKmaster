#include "stepmotor.h"
#include "stdio.h"
#include "math.h"
u32 PUL_CNT;   // TIM3脉冲计数
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
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //GPIO时钟使能
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //定时器3时钟使能 
 //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //定时器2时钟使能
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;    //PA7为TIM3通道2
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;  //复用推免输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //GPIO口响应速度 
 GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;    //PA6为DIR控制输出
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  //推免输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //GPIO口响应速度 
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	
	//TIM3_Configuration
 TIM_TimeBaseStructure.TIM_Period            = 23999;     //自动重装载寄存器
 TIM_TimeBaseStructure.TIM_Prescaler         = 2;      //预分频器,t=(23999+1)*(2+1)/72M
 TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up; //计数器向上计数模式     
 TIM_TimeBaseStructure.TIM_ClockDivision     = 0x0;      //时钟分频因子
 TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;      //每次溢出都产生事件更新
 TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);       //写TIM3各寄存器参数
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 TIM_ClearFlag(TIM3,TIM_FLAG_Update);          //中断标志位清零
 TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);       //允许捕获/比较3中断
 TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;   //PWM模式2 TIM3_CCMR1[14:12]=111 在向上计数时，一旦TIMx_CNT
 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输入/捕获2输出允许
 TIM_OCInitStructure.TIM_Pulse       = 11000;       //确定占空比，这个值决定了有效电平的时间。
 TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;      //输出极性,低电平有效   
 TIM_OC2Init(TIM3, &TIM_OCInitStructure);         //配置定时器输出模式,比较参数等
 TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);       //使能TIM3在CCR2上的预装载寄存器

 
 //TIM2_Configuration ////////////////////////////////////////////////////////////////
// TIM_DeInit(TIM2);          //TIM2重新配置为缺省值，默认状态
// TIM_TimeBaseStructure.TIM_Period        = 359;     //自动重装载寄存器
// TIM_TimeBaseStructure.TIM_Prescaler     = 199;     //时钟预分频器
// TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //计数器向上计数模式          
// TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;         //时钟分频因子 
// TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);     //配置TIM2寄存器各参数
// TIM_ClearFlag(TIM2,TIM_FLAG_Update);          //中断标志位清零
// TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);       //允许捕获/比较2中断
 //////////////////////////////////////////////////////////////////////////////////////
 NVIC_InitStructure.NVIC_IRQChannel          = TIM3_IRQn ;  //选择定时器TIM3
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;          //选择抢先式优先级（与中断嵌套级别有关）
 NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;       //选择子优先级（同抢先式优先级的响应顺序）
 NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;    //选择使能中断源
 NVIC_Init(&NVIC_InitStructure);
 NVIC_InitStructure.NVIC_IRQChannel          = TIM2_IRQn ;  //选择定时器TIM2
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;          //选择抢先式优先级（与中断嵌套级别有关）
 NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;       //选择子优先级（同抢先式优先级的响应顺序）
 NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;    //选择使能中断源
 NVIC_Init(&NVIC_InitStructure);
}
void TIM3_Configuration(u32 period)
{
// TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 //TIM_OCInitTypeDef TIM_OCInitStructure;
	
 TIM3->ARR = period-1;
 TIM3->CCR2 = period >>2;
// TIM3->PSC = 7;
 //TIM_TimeBaseStructure.TIM_Period            = period-1;      //自动重装载寄存器
 //TIM_TimeBaseStructure.TIM_Prescaler         = 29;    //预分频器,f=72M/[(period+1)*(29+1)], ft = 2400000
 //TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up; //计数器向上计数模式     
 //TIM_TimeBaseStructure.TIM_ClockDivision     = 0x0;      //时钟分频因子
 //TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;      //每次溢出都产生事件更新
// TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);       //写TIM3各寄存器参数
 //TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;   //PWM模式2 TIM3_CCMR1[14:12]=111 在向上计数时，一旦TIMx_CNT
// TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输入/捕获2输出允许
 //TIM_OCInitStructure.TIM_Pulse       = period >> 2;     //确定占空比，25%
 //TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;      //输出极性,低电平有效   
 //TIM_OC2Init(TIM3, &TIM_OCInitStructure);         //配置定时器输出模式,比较参数等
}




//void MOTOR_RUN(u32 acc, u32 dec, u32 topspeed, u32 dis)
//步进电机运行参数
//acc -- 加速度,单位: round/min/s
//dec -- 减速度,单位: round/min/s
//topspeed -- 最高速度,单位: round/min
//dis -- 总角位移,单位: round/10000
void MOTOR_RUN(u32 acc, u32 dec, u32 topspeed, u32 dis)
{
  u32 t_acc,t_dec,step_all,step_acc,step_dec,step_run;
  u32 i,tim_cnt,tim_rest,tim_cnt_temp;
  
  step_all = (float)dis * (N_MOTOR * 0.0001);
  t_acc = topspeed * 1000 / acc; //unit: ms
  t_dec = topspeed * 1000 / dec; //unit: ms
  if(topspeed * (t_acc + t_dec) / 12 > dis)  //达不到最高速度 // topspeed/60/1000 * (t_acc + t_dec) / 2 > dis / 10000
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

