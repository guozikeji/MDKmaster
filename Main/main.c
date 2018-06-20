
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "timer.h"
extern u16 WorkNumber_Value;
extern u8 Start_Flag;
extern u16  Speed_Value;
extern u16  Length_Value ;
u8  OutputLedStatus  = 0;
u8  OnceFlag = 0;
int main(void)
{
  BspInit();
	BspIwdgConfiguration();	
	TIM3_PWM_Init(999,72);
  Step_Enable = ON;
 while (1)
	{	 
	  IWDG_ReloadCounter();       //喂狗
	  Rx232ProcessFunc();
		led_display();
	  StepRun_Func(Start_Flag, Speed_Value, Length_Value,&WorkNumber_Value);
		StepOnce_Func(OnceFlag,Speed_Value, Length_Value);
		Rs232SendData_Run_Func(WorkNumber_Value);
		Output_display(OutputLedStatus);
	}
}



// /****************************************************************************

//****************************************************************************/
//int fputc(int ch, FILE *f)
//{
//  /* Place your implementation of fputc here */
//  /* e.g. write a character to the USART */
//  USART_SendData(UART4, (u8) ch);
//
//  /* Loop until the end of transmission */



//  while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET)
//  {}
//  return ch;
//}
//
//// 接收数据
//
//int GetKey (void) { 
//
//while (!(UART4->SR & USART_FLAG_RXNE));
//
//return ((int)(UART4->DR & 0x1FF));
//
//}
 /****************************************************************************

****************************************************************************/
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (u8) ch);

  /* Loop until the end of transmission */
  while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
  return ch;
}



int GetKey (void) { 

while (!(USART2->SR & USART_FLAG_RXNE));

return ((int)(USART2->DR & 0x1FF));

}



//#ifdef  USE_USART1
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
//#endif

