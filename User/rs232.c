#include "rs232.h"
#include "common.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////  定义全局变量
u16  Speed_Value = 0;
u16  Length_Value = 0;
u16  WorkNumber_Value = 0;              //  定义  工作数量
u8   Rs232RxProcessBuffer[RS232_BUFFER_SIZE];
u8   Rs232ProcessLength = 0;
u8   High = 0 , Low = 0;
u8   Start_Flag;
u8   PowerOn_Flag = 0;
u8   Mode = 0;
//////////////////////////////////////////////////////////////////////  定义外部变量
extern u16 Rs232ReceiveCount;
extern u8  Rs232RxBuffer[];
extern u8  Rs232RxComplete;
extern u8  Rs232TxBuffer[];
extern u16 SendDataTimer;
extern u8  OutputLedStatus;
extern u8  NoneTapeFlag ;
extern u8  OnceFlag;
////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////////// 串口处理程序
void Rx232ProcessFunc()
{
 u8 i = 0,cs = 0,flag = 0  ;
// if(Rs232ReceiveCount > 0 && Rs232RxBuffer[Rs232ReceiveCount-1] == Ending_Code )
// {
//	for(i=0; i<Rs232ReceiveCount; i++)
//	{
//	 Rs232RxProcessBuffer[i] = Rs232RxBuffer[i];
//	}
//  Rs232ProcessLength = i;
//	i = 0;
//	BspRs232Clear();                  //  清除串口数据
	
 if(Rs232ReceiveCount > 0 && Rs232RxBuffer[Rs232ReceiveCount-1] == Ending_Code )
 {
	for(i=0; i<Rs232ReceiveCount-2; i++)
	{
	 Rs232RxProcessBuffer[i] = Rs232RxBuffer[i];
	 cs += Rs232RxProcessBuffer[i];
	}
	Rs232RxProcessBuffer[8] = Rs232RxBuffer[8];
	if(Rs232RxProcessBuffer[8] == cs)
	{
	 flag = 1;
	}
  Rs232ProcessLength = i+2;
	i = 0;
	BspRs232Clear();
  if(flag)
	{		
	do
  { 
	 while(i < Rs232ProcessLength)
	 {
	 if(Rs232RxBuffer[i] == Start_Code)
	 {
	  break;
	 }
	 else i++;
	 }                        //   在次验证码对不对
	 i++;
	  if(i < Rs232ProcessLength)
	  {
	   switch(Rs232RxProcessBuffer[i])
		 {
			case Config_Code : 
			i++;
		  i = ConfigProcessFunc(i);
			PowerOn_Flag = 1;
			Rs232SendData();
			break;
		  case Once_Code :
			i++;
			i = OnceWorkingProcessFunc(i);
			break;
			default : break;
		 }
	  }
	} while(i < Rs232ProcessLength);
 }
 }	
}
//////////////////////////////////////////////////////////////////////////////////  读配置程序
u8 ConfigProcessFunc(u8 tmp)
{
 WorkNumber_Value  =   Rs232RxProcessBuffer[tmp++]; 
 WorkNumber_Value |=   (Rs232RxProcessBuffer[tmp++] << 8) ;
 Length_Value       = 	 Rs232RxProcessBuffer[tmp++]; 
 Length_Value      |=   (Rs232RxProcessBuffer[tmp++] <<8);
 Speed_Value      =   Rs232RxProcessBuffer[tmp++] ; 
 Speed_Value     |=   (Rs232RxProcessBuffer[tmp++]<< 8);
 Start_Flag = ON;
 OutputLedStatus = 0;
 NoneTapeFlag = 0 ;
 Mode = 0;
 return tmp;
}
/////////////////////////////////////////////////////////////////////////////////// 动作一次处理程序u8
u8 OnceWorkingProcessFunc(u8 tmp)
{
 WorkNumber_Value  =   Rs232RxProcessBuffer[tmp++]; 
 WorkNumber_Value |=   (Rs232RxProcessBuffer[tmp++] << 8) ;
 Length_Value       = 	 Rs232RxProcessBuffer[tmp++]; 
 Length_Value      |=   (Rs232RxProcessBuffer[tmp++] <<8);
 Speed_Value      =   Rs232RxProcessBuffer[tmp++] ; 
 Speed_Value     |=   (Rs232RxProcessBuffer[tmp++]<< 8);
 OnceFlag = ON;
 Mode = 1;
 return tmp;
}

//////////////////////////////////////////////////////////////////////////////////    串口发送程序
void Rs232SendData(void)
{
  u8 index = 0;
  u8 cs = 0;	
	Rs232TxBuffer[index] = Start_Code;              cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = Config_Code;             cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = WorkNumber_Value;        cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = WorkNumber_Value << 8;   cs += Rs232TxBuffer[index++];
  Rs232TxBuffer[index] = Rs232RxProcessBuffer[4]; cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = Rs232RxProcessBuffer[5]; cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = Rs232RxProcessBuffer[6]; cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = Rs232RxProcessBuffer[7]; cs += Rs232TxBuffer[index++];
  Rs232TxBuffer[index++] = cs;
	Rs232TxBuffer[index++] = Ending_Code;
  BspRs232Write(index);
}
//////////////////////////////////////////////////////////////////////////////////////  串口zero data
void Rs232SendZeroData(void)
{
  u8 index = 0;
  u8 cs = 0;	
	Rs232TxBuffer[index] = Start_Code;              cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = Config_Code;             cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = 0;        cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = 0;   cs += Rs232TxBuffer[index++];
  Rs232TxBuffer[index] = 0; cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = 0; cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = 0; cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = 0; cs += Rs232TxBuffer[index++];
  Rs232TxBuffer[index++] = cs;
	Rs232TxBuffer[index++] = Ending_Code;
  BspRs232Write(index);
}
////////////////////////////////////////////////////////////////////////////////////
void Rs232SendNoneData(void)
{
  u8 index = 0;
  u8 cs = 0;	
	Rs232TxBuffer[index] = Start_Code;              cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = 0;             cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = 0;        cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = 0;   cs += Rs232TxBuffer[index++];
  Rs232TxBuffer[index] = 0; cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = 0; cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = 0; cs += Rs232TxBuffer[index++];
	Rs232TxBuffer[index] = 0; cs += Rs232TxBuffer[index++];
  Rs232TxBuffer[index++] = cs;
	Rs232TxBuffer[index++] = Ending_Code;
  BspRs232Write(index);
}
/////////////////////////////////////////////////////////////////////////////////////
void Rs232SendData_Run_Func(u16 work_number)
{
 if(work_number == 0 && PowerOn_Flag == TRUE && NoneTapeFlag == RESET && Mode == 0)    {Rs232SendZeroData();}
 else if(work_number >0 && PowerOn_Flag == TRUE && NoneTapeFlag == RESET && Mode == 0) {Rs232SendData();  }
 else if(NoneTapeFlag == TRUE && PowerOn_Flag == TRUE && Mode == 0)                    {Rs232SendNoneData();} 
 SendDataTimer = 0; while(SendDataTimer < 20);
}
