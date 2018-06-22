
#include "bsp_adc.h"
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/** @defgroup ADC_Private_Variables
  * @{
  */
 #define BSP_ADC1_DR_ADDRESS    ((uint32_t)0x4001244C) //ADC1
u16  BspAdcConvertedBuffer[16];
u16  BspAdcTempData;
u16 BspAdcRead(void)
{
	u16 i,j;
	u32 temp;
	for(i=0;i<15;i++)
	{
		for(j=i+1;j<16;j++)if(BspAdcConvertedBuffer[i]>BspAdcConvertedBuffer[j])
		{
			temp=BspAdcConvertedBuffer[i];
			BspAdcConvertedBuffer[i]=BspAdcConvertedBuffer[j];
			BspAdcConvertedBuffer[j]=temp;
		}
             //  XIAO TATA
	}
	temp=0;
	for(i=4;i<12;i++)temp+=BspAdcConvertedBuffer[i];
	BspAdcTempData=temp>>3;
	temp=BspAdcTempData;
	temp=(143000 - temp*330000/4096)/43 + 250;
	return temp;	
}
void BspAdcConfig(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	/* Enable ADC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1  , ENABLE);
	
	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);   
	DMA_InitStructure.DMA_PeripheralBaseAddr = BSP_ADC1_DR_ADDRESS; 
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)BspAdcConvertedBuffer; 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  
	DMA_InitStructure.DMA_BufferSize = 16;  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable ;  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;  
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_NbrOfChannel = 16; 
	ADC_Init(ADC1, &ADC_InitStructure);	  /* Enable ADC1 DMA */
	
	 
	/* ADC1 regular channel8 configuration  */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5   );	 //AIW0	 温度
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 2, ADC_SampleTime_239Cycles5   );	 //AIW0	 温度
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 3, ADC_SampleTime_239Cycles5   );	 //AIW0	 温度
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 4, ADC_SampleTime_239Cycles5   );	 //AIW0	 温度

	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 5, ADC_SampleTime_239Cycles5   );	 //AIW0	 温度
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 6, ADC_SampleTime_239Cycles5   );	 //AIW0	 温度
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 7, ADC_SampleTime_239Cycles5   );	 //AIW0	 温度
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 8, ADC_SampleTime_239Cycles5   );	 //AIW0	 温度

	ADC_RegularChannelConfig(ADC1, ADC_Channel_16 , 9, ADC_SampleTime_239Cycles5   );  //AIW1	 压力
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16 , 10, ADC_SampleTime_239Cycles5   );  //AIW1	 压力
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16 , 11, ADC_SampleTime_239Cycles5   );  //AIW1	 压力
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16 , 12, ADC_SampleTime_239Cycles5   );  //AIW1	 压力

	ADC_RegularChannelConfig(ADC1, ADC_Channel_16 , 13, ADC_SampleTime_239Cycles5   );  //AIW1	 压力
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16 , 14, ADC_SampleTime_239Cycles5   );  //AIW1	 压力
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16 , 15, ADC_SampleTime_239Cycles5   );  //AIW1	 压力
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16 , 16, ADC_SampleTime_239Cycles5   );  //AIW1	 压力


	/* ADC1 configuration ------------------------------------------------------*/
	ADC_DMACmd(ADC1, ENABLE);	//
	
//	/* Enable TempSensor ------------------------------------------------------*/
	ADC_TempSensorVrefintCmd(ENABLE);
	/* Enable JEOC interupt */
//    ADC_ITConfig(ADC1, ADC_IT_EOC , ENABLE);
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE); 
	
	/* Enable ADC1 reset calibaration register 复位校准寄存器*/   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));  
	
	/* Start ADC1 calibaration 始校准*/
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */                                  
	while(ADC_GetCalibrationStatus(ADC1));
	 
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//	BspAdcNvicInit();
}
void BspAdcNvicInit(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;  
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}
