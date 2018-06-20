

#include "rs485two.h"

extern u16 const RS485_BUFFER2_SIZE;
extern u8 RS485_RX2_BUFFER[];
extern u8 RS485_TX2_BUFFER[];
extern u16 RS485_Receive2Count;
extern u8 RS485_RX2_COMPLETE;
extern u16 RS485_Time20,RS485_Time21;

extern u8 Rs232InputHex[8][4];
extern u8 RS232OutHex[8][4];

u8 Rs485TwoLoopIndex=1;
u8 Rs485TwoSendCount=0;

u8 Rs485TwoReceive(void)
{
	u8 flag=0,cs=0;
	u16 i;
	if(RS485_RX2_COMPLETE)
	{	
		if(RS485_Receive2Count>5)
		{
			for(i=0;i<(RS485_Receive2Count-2);i++)
			{
				cs+=RS485_RX2_BUFFER[i];
			}
		}
		if(cs==RS485_RX2_BUFFER[RS485_Receive2Count-2])flag=1;
		Bsp2Rs485Clear();	
		i=RS485_RX2_BUFFER[2]<<1;
		if((flag)&& (i>0)&&(i<7))
		{
			Rs232InputHex[i][0]=RS485_RX2_BUFFER[3];
			Rs232InputHex[i][1]=RS485_RX2_BUFFER[4];
			Rs232InputHex[i][2]=RS485_RX2_BUFFER[5];
			Rs232InputHex[i][3]=RS485_RX2_BUFFER[6];
			Rs232InputHex[i+1][0]=RS485_RX2_BUFFER[7];
			Rs232InputHex[i+1][1]=RS485_RX2_BUFFER[8];
			Rs232InputHex[i+1][2]=RS485_RX2_BUFFER[9];
			Rs232InputHex[i+1][3]=RS485_RX2_BUFFER[10];		
			Rs485TwoLoopIndex++;Rs485TwoSendCount=0;			
		}
	}
	if(RS485_Time20>10){Bsp2Rs485Clear();}
  return flag;	
}

void Rs485TwoSendLoop(u8 command)
{
	
	if(Rs485TwoSendCount>3){Rs485TwoSendCount=0;Rs485TwoLoopIndex++;}
	if(RS485_Time21>100)
	{
		switch(Rs485TwoLoopIndex)
		{
			case 1: Rs485TwoSend(command);Rs485TwoSendCount++;break;
			case 2: Rs485TwoSend(command);Rs485TwoSendCount++;break;
			case 3: Rs485TwoSend(command);Rs485TwoSendCount++;break;
			default: Rs485TwoLoopIndex=1;break;
		}
		RS485_Time21=0;RS485_Time20=0;	
	}	
}
void Rs485TwoSend(u8 command)
{
		u8 cs=0;	
		u8 index=Rs485TwoLoopIndex<<1;
		RS485_TX2_BUFFER[0] =START_CODE; cs+=RS485_TX2_BUFFER[0];
		RS485_TX2_BUFFER[1] =command;    cs+=RS485_TX2_BUFFER[1];	
		RS485_TX2_BUFFER[2] =Rs485TwoLoopIndex;cs+=RS485_TX2_BUFFER[2];	
		RS485_TX2_BUFFER[3] =RS232OutHex[index][0];	cs+=RS485_TX2_BUFFER[3];
		RS485_TX2_BUFFER[4] =RS232OutHex[index][1];	cs+=RS485_TX2_BUFFER[4];
		RS485_TX2_BUFFER[5] =RS232OutHex[index][2];	cs+=RS485_TX2_BUFFER[5];
		RS485_TX2_BUFFER[6] =RS232OutHex[index][3];	cs+=RS485_TX2_BUFFER[6];
		RS485_TX2_BUFFER[7] =RS232OutHex[index+1][0];	cs+=RS485_TX2_BUFFER[7];
		RS485_TX2_BUFFER[8] =RS232OutHex[index+1][1];	cs+=RS485_TX2_BUFFER[8];
		RS485_TX2_BUFFER[9] =RS232OutHex[index+1][2];	cs+=RS485_TX2_BUFFER[9];
		RS485_TX2_BUFFER[10]=RS232OutHex[index+1][3];	cs+=RS485_TX2_BUFFER[10];		
		RS485_TX2_BUFFER[11] =cs;		
		RS485_TX2_BUFFER[12]=END_CODE;
		Bsp2Rs485Write(13);		
}
