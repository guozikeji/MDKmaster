

#include "rs485.h"
#include "common.h"
#include "rs232.h"


extern u8 Rs485RxBuffer[];
extern u8 Rs485TxBuffer[];
extern u16 Rs485ReceiveCount;
extern u8 Rs485RxComplete;
extern u16 Rs485Time1;


u8 Rs485SetOutBuffer[4][12];
u8 Rs485ReadInBuffer[8][12];
u8 Rs485SetStatusBuffer[8][12];
u16 Rs485OnLineTime=0;
u8 Rs485LoopStep=0;
u8 Rs485LoopSiteNumber=0;
u8 SiteMaxNumber=0;
u16 SiteWaitTimeSet=20;
u16 SiteWaitTime=0;
u16 Rs485LoopWaitTime=0;
u16 Rs485LoopWaitTimeSet = 100 ;
u8 Rs485SendCount=0;
u8 Rs485SendCountSet=3;
u8 Rs485LedFlag=0;

u8 SiteRestartCount;
u8 SiteRestartCountSet=2;

u8 SiteBuffer[8];
u8 SiteNumber;
u8 Status = 0x31;
extern u8 PortLedFlag;
extern u8 ReadyLedFlag;
extern u8 Rs232SetTxBuffer[20];
u8 Rs485ProcessRStart = 0;
u8 Rs485Flag = 0;
u8 Rs485SendLoopFlag = FALSE;
u8 PortError =0;

//自动搜索保存 0-F 站号及 在线从机数

void Rs485SendLoop(u8 status)
{
	u8 i,cs=0;
 if(Rs485LoopSiteNumber>=SiteNumber){Rs485LoopStep=0;Rs485LoopSiteNumber=0;}
	switch(Rs485LoopStep)
	{
		case 0:
				    Rs485SendData(status);
						Rs485LoopStep++;Rs485LoopWaitTime=0;
						break;
		case 1:
				   	if(Rs485RxComplete)
						{				
							if((Rs485RxBuffer[1]==SiteBuffer[Rs485LoopSiteNumber])&(Rs485RxBuffer[1]<16))
							{
									for(i=0;i<(Rs485RxBuffer[2]+8);i++){cs+=Rs485RxBuffer[i];Rs485ReadInBuffer[SiteBuffer[Rs485LoopSiteNumber]][i]=Rs485RxBuffer[i];}
									if((cs==Rs485RxBuffer[i])&&(Rs485RxBuffer[i+1]==END_CODE))
									{
										Rs232SetTxBuffer[4]= Rs485RxBuffer[4];
										Rs232SetTxBuffer[5]= Rs485RxBuffer[5];
										Rs232SetTxBuffer[6]= Rs485RxBuffer[6];
										Rs232SetTxBuffer[7]= Rs485RxBuffer[7];
										Rs232SetTxBuffer[8]= Rs485RxBuffer[8];
										Rs232SetTxBuffer[9]= Rs485RxBuffer[9];
									}								
							}				
							Rs485LoopStep=0;
							Rs485LoopSiteNumber++;
							Rs485LedFlag=1;
							Rs485SendCount=0;
							BspRs485Clear();
							break;
								
						}
						if(Rs485Time1>10)BspRs485Clear();
						if(Rs485LoopWaitTime>Rs485LoopWaitTimeSet)
						{
							Rs485SendCount++;
						}
						if(Rs485SendCount>Rs485SendCountSet)
						{
								Rs485LoopSiteNumber++;Rs485SendCount=0;Rs485LoopStep=0;
						}
				   break;
		       default:
						Rs485LoopSiteNumber++;Rs485SendCount=0;Rs485LoopStep=0;
					 break;
	}
 }
						
void Rs485SendData(u8 status)
{
	u8 i=0,cs=0;
	Rs485TxBuffer[i]=START_CODE; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=Rs485LoopSiteNumber; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=2; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=status; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=Rs485SetOutBuffer[SiteBuffer[Rs485LoopSiteNumber]][0]; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=Rs485SetOutBuffer[SiteBuffer[Rs485LoopSiteNumber]][1]; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=Rs485SetOutBuffer[SiteBuffer[Rs485LoopSiteNumber]][2]; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=Rs485SetOutBuffer[SiteBuffer[Rs485LoopSiteNumber]][3]; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=Rs485SetOutBuffer[SiteBuffer[Rs485LoopSiteNumber]][4]; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=Rs485SetOutBuffer[SiteBuffer[Rs485LoopSiteNumber]][5]; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i++]=cs;
	Rs485TxBuffer[i++]=END_CODE;	
	BspRs485Write(i);	
}
void Rs485SendZeroData(u8 site)
{
	u8 i=0,cs=0;
	Rs485TxBuffer[i]=START_CODE; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=site; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=2;    cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=0x31; cs+=Rs485TxBuffer[i++];// 状态位
	Rs485TxBuffer[i]=0; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=0; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=0; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=0; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=0; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=0; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i++] = cs;
	Rs485TxBuffer[i++]=END_CODE;
  Rs485LedFlag =1;
	BspRs485Write(i);
}
void Rs485_Get_Site(void)
{
	u8 i,j,flag=0,cs=0;
	SiteMaxNumber=0;
	for(i=0;i<BOARD_MAX;i++)
	{
		SiteRestartCount=0;	
		while(SiteRestartCount<SiteRestartCountSet)
		{
				Rs485SendZeroData(i);Rs485Time1=0;SiteWaitTime=0;
			  while(SiteWaitTime<SiteWaitTimeSet){if(Rs485RxComplete)break;}
				if(Rs485RxComplete)
				{
				 if(Rs485RxBuffer[3] == Get_Site_Common )
					{	 	
					cs=0;
					for(j=0;j<(Rs485RxBuffer[2]+8);j++){cs+=Rs485RxBuffer[j];}
					if((cs==Rs485RxBuffer[j])&&(Rs485RxBuffer[j+1]==END_CODE))
					{
						SiteBuffer[SiteNumber]=Rs485RxBuffer[1];	
            if(SiteBuffer[SiteNumber]>SiteMaxNumber)SiteMaxNumber=SiteBuffer[SiteNumber];						
						SiteRestartCount=0;
						Rs485LedFlag=1;
						flag=1;
						break;
					}
				}
			}	
				SiteRestartCount++;
		}
		if(flag)
		{
			SiteNumber++;
			
			flag=0;
		}
		BspRs485Clear();
	}
	SiteMaxNumber+=1;
//	SiteBuffer[0]=0x07;
	if(SiteMaxNumber>BOARD_MAX)SiteMaxNumber=BOARD_MAX;	
}
void Rs485SendProcessData(u8 status,u8 number,u8 SiteNumber)
{
  u8 i=0,cs=0;
	Rs485TxBuffer[i]=START_CODE; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=SiteNumber; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=2; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=Status; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=Rs485SetStatusBuffer[number][0]; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=Rs485SetStatusBuffer[number][1]; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=Rs485SetStatusBuffer[number][2]; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=Rs485SetStatusBuffer[number][3]; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=Rs485SetStatusBuffer[number][4]; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i]=Rs485SetStatusBuffer[number][5]; cs+=Rs485TxBuffer[i++];
	Rs485TxBuffer[i++]=cs;
	Rs485TxBuffer[i++]=END_CODE;	
	BspRs485Write(i);	
}

