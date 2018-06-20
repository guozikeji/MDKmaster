
/* Includes ------------------------------------------------------------------*/
#define BSP_FLASH_GLOBALS

#include "bsp_flash.h"
  /* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

//STM32F103VCT6


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/  
//读取FLASH数据-成功返回0
u8 BspFlashInit(void)
{
	u8 i,j;
	BspFlashRead();
	for(j=0;j<3;j++)
	{
		if(FlashInitFlag!=(*(__IO uint16_t*) (RamEndAddr-2)))
		{
			BspRawInit();
			for(i=0;i<3;i++)if(BspFlashWrite()==0)break;
			if(i>=3)return 1;
		}else break;
	}
	if(j>=3)return 1;
	return 0;
}
//初始化时写入基础数据
void BspRawInit(void)
{
		//初始化数据标志
		(*(__IO uint16_t*) (RamEndAddr-2))=FlashInitFlag;
	  //贝特仪表
		(*(__IO uint16_t*)RamStartAddr        )=0X0001;			//表1贝特	
		(*(__IO uint16_t*) (RamStartAddr+  2 ))=0x6104;			//表号 10020461
		(*(__IO uint16_t*) (RamStartAddr+  4 ))=0x0210;			//表号
		(*(__IO uint16_t*) (RamStartAddr+  6 ))=0x0011;			//表号附加码 00 11 11
		(*(__IO uint16_t*) (RamStartAddr+  8 ))=0x1100;			//表号附加码 00 11 11
		(*(__IO uint16_t*) (RamStartAddr+ 10 ))=0xA001;			//单元号
		(*(__IO uint16_t*) (RamStartAddr+ 12 ))=0xF001;			//房间号
		(*(__IO uint16_t*) (RamStartAddr+ 14 ))=0x1000;			//室内表号
		(*(__IO uint16_t*) (RamStartAddr+ 16 ))=0x0001;			//室内表号
		(*(__IO uint16_t*) (RamStartAddr+ 18 ))=2000;			  //设定温度20.00度
		(*(__IO uint16_t*) (RamStartAddr+ 20 ))=0x2000;			//阀表号
		(*(__IO uint16_t*) (RamStartAddr+ 22 ))=0x0001;			//阀表号
		(*(__IO uint16_t*) (RamStartAddr+ 24 ))=1000;			  //阀开度100.0%
		(*(__IO uint16_t*) (RamStartAddr+ 26 ))=1;			    //阀控制模式
	  //迈拓仪表
		(*(__IO uint16_t*) (RamStartAddr+ 28 ))=0X0002;			//表2迈拓	
		(*(__IO uint16_t*) (RamStartAddr+ 30 ))=0x0147;			//表号 01470759
		(*(__IO uint16_t*) (RamStartAddr+ 32 ))=0x0759;			//表号
		(*(__IO uint16_t*) (RamStartAddr+ 34 ))=0x0011;			//表号附加码 00 11 11
		(*(__IO uint16_t*) (RamStartAddr+ 36 ))=0x1100;			//表号附加码 00 11 11
		(*(__IO uint16_t*) (RamStartAddr+ 38 ))=0xA002;			//单元号
		(*(__IO uint16_t*) (RamStartAddr+ 40 ))=0xF002;			//房间号
		(*(__IO uint16_t*) (RamStartAddr+ 42 ))=0x1000;			//室内表号
		(*(__IO uint16_t*) (RamStartAddr+ 44 ))=0x0002;			//室内表号
		(*(__IO uint16_t*) (RamStartAddr+ 46 ))=2000;			  //设定温度20.00度
		(*(__IO uint16_t*) (RamStartAddr+ 48 ))=0x2000;			//阀表号
		(*(__IO uint16_t*) (RamStartAddr+ 50 ))=0x0002;			//阀表号
		(*(__IO uint16_t*) (RamStartAddr+ 52 ))=1000;			  //阀开度100.0%
		(*(__IO uint16_t*) (RamStartAddr+ 54 ))=1;			    //阀控制模式
	  //LANDIS 2WR6
		(*(__IO uint16_t*) (RamStartAddr+ 28 ))=0X0040;			//表 0X40 LANDIS 2WER6
		(*(__IO uint16_t*) (RamStartAddr+ 30 ))=0x9630;			//表号 66073096
		(*(__IO uint16_t*) (RamStartAddr+ 32 ))=0x0766;			//表号
		(*(__IO uint16_t*) (RamStartAddr+ 34 ))=0xA732;			//表号附加码 A7 32 03 04
		(*(__IO uint16_t*) (RamStartAddr+ 36 ))=0x0304;			//表号附加码 A7 32 03 04
		(*(__IO uint16_t*) (RamStartAddr+ 38 ))=0xA003;			//单元号
		(*(__IO uint16_t*) (RamStartAddr+ 40 ))=0xF003;			//房间号
		(*(__IO uint16_t*) (RamStartAddr+ 42 ))=0x1000;			//室内表号
		(*(__IO uint16_t*) (RamStartAddr+ 44 ))=0x0003;			//室内表号
		(*(__IO uint16_t*) (RamStartAddr+ 46 ))=2000;			  //设定温度20.00度
		(*(__IO uint16_t*) (RamStartAddr+ 48 ))=0x2000;			//阀表号
		(*(__IO uint16_t*) (RamStartAddr+ 50 ))=0x0003;			//阀表号
		(*(__IO uint16_t*) (RamStartAddr+ 52 ))=1000;			  //阀开度100.0%
		(*(__IO uint16_t*) (RamStartAddr+ 54 ))=1;			    //阀控制模式
	
		(*(__IO uint16_t*) RamBulidingAddr     )=0X3132;   //楼号高	  840 ASCII 1234
		(*(__IO uint16_t*) (RamBulidingAddr+2) )=0X3334;   //楼号高	  840 ASCII
	
		(*(__IO uint16_t*) RamMeterAddr        )=0;   //热表总数 842
		(*(__IO uint16_t*) RamUserAddr         )=1;   //热表总数 843
		(*(__IO uint16_t*) RamCircuitChangeAddr)=30;   //线路切换时间 30S
		(*(__IO uint16_t*) RamRunModeAddr      )=60;   //运行模式 
		(*(__IO uint16_t*) RamMinValveAddr     )=100;   // 防冻开度10%
		(*(__IO uint16_t*) RamMinTempRoomAddr  )=1600;   // 防冻温度16度
		(*(__IO uint16_t*) RamIntegralTimeAddr )=800;   // 积分时间  800=100S

		(*(__IO uint16_t*)  RamMbusStartAddr    )=0X0B0A; 	   //MBUS 起始月   (11.10-3.20)
		(*(__IO uint16_t*)  RamMbusEndAddr			)=0X0314;    //MBUS 起始月    (11.10-3.20)
		(*(__IO uint16_t*)  RamTimeSection1Addr	)=0X0100; 	   //上传时间段1 小时  分钟
		(*(__IO uint16_t*)  RamTimeSection2Addr	)=0X0400; 	   //上传时间段2 小时  分钟
		(*(__IO uint16_t*)  RamTimeSection3Addr	)=0X0700; 	   //上传时间段3 小时  分钟
		(*(__IO uint16_t*)  RamTimeSection4Addr	)=0X0A00; 	   // 上传时间段4 小时  分钟
		(*(__IO uint16_t*)  RamTimeSection5Addr	)=0X0D00; 	   // 上传时间段5 小时  分钟
		(*(__IO uint16_t*)  RamTimeSection6Addr	)=0X1000; 	   // 上传时间段6 小时  分钟
		(*(__IO uint16_t*)  RamTimeSection7Addr	)=0X1300;    // 上传时间段7 小时  分钟
		(*(__IO uint16_t*)  RamTimeSection8Addr	)=0X1600; 	   // 上传时间段8 小时  分钟

		(*(__IO uint16_t*)  RamMbusSubpackageTimeAddr )=100; 	   //MBUS 分包时间 地位ms
		(*(__IO uint16_t*)  RamMbusBaudRateAddr		    )=2400;    //MBUS 波特率
		(*(__IO uint16_t*)  RamMbusDateBitsAddr	      )=0X1000;   //MBUS 数据位 可设8-0x0000 或9-0x1000
		(*(__IO uint16_t*)  RamMbusStopBitsAddr	      )=0;    //1停止位0X0000 0.5 停止位 0X1000 2停止位 0X2000 1.5停止位0X3000
		(*(__IO uint16_t*)  RamMbusParityBitsAddr     )=0X0400; 	   //0X0000无校验 0X0600奇校验  0X0400偶校验
		(*(__IO uint16_t*)  RamMbusRestartTimeAddr	  )=5000;    // MBUS 重启时间 5s   
		(*(__IO uint16_t*)  RamMbusCircuitTimeAddr	  )=3000; 	   // MBUS 循环时间 3S
		(*(__IO uint16_t*)  RamMbusErrorCountAddr		  )=4;    // MBUS 读取错误次数	4

		(*(__IO uint16_t*)  RamLcdControlAddr           )=0; 	   //0本地控制，1远程控制
		(*(__IO uint16_t*)  RamLcdRefreshTimeAddr		    )=5;    //LCD刷屏时间 	5
		(*(__IO uint16_t*)  RamLcdStartTimeAddr	        )=0; 	   // LCD显示开始时间 7:30开始显示
		(*(__IO uint16_t*)  RamLcdEndTimeAddr	          )=0x181e;; 	   // LCD显示结束时间 18:30结束显示
		(*(__IO uint16_t*)  RamLcdHeadFrontColorAddr    )=0xf7de;    //LCD头文件前景色 灰色
		(*(__IO uint16_t*)  RamLcdHeadBackColorAddr	    )=0xF800; 	   //LCD头文件背景色 红色
		(*(__IO uint16_t*)  RamLcdContentFrontColorAddr	)=0xf7de;    //LCD内容前景色 灰色
		(*(__IO uint16_t*)  RamLcdContentBackColorAddr  )=0xF81F; 	   //LCD内容背景色	洋红
		(*(__IO uint16_t*)  RamLcdHighLightColorAddr	  )=0x051F;    //LCD高亮显示色	蓝色
		(*(__IO uint16_t*)  RamLcdBrightnessAddr        )=800;    //LCD高亮 比例 0灭 1000最大

		(*(__IO uint16_t*)  RamRs485SubpackageTimeAddr )=10;    //RS485分包时间
		(*(__IO uint16_t*)  RamRs485BaudRateAddr		   )=9600;    // 波特率
		(*(__IO uint16_t*)  RamRs485DateBitsAddr	     )=0;    // 数据位 可设8-0x0000 或9-0x1000
		(*(__IO uint16_t*)  RamRs485StopBitsAddr	     )=0;    //1停止位0X0000 0.5 停止位 0X1000 2停止位 0X2000 1.5停止位0X3000
		(*(__IO uint16_t*)  RamRs485ParityBitsAddr     )=0;    //0X0000无校验 0X0600奇校验  0X0400偶校验

		(*(__IO uint16_t*)  RamGprsSubpackageTimeAddr  )=10;   //GPRS分包时间
		(*(__IO uint16_t*)  RamGprsBaudRateAddr		     )=9600;    // 波特率
		(*(__IO uint16_t*)  RamGprsDateBitsAddr	       )=0; 	   // 数据位 可设8-0x0000 或9-0x1000
		(*(__IO uint16_t*)  RamGprsStopBitsAddr	       )=0; 	   //1停止位0X0000 0.5 停止位 0X1000 2停止位 0X2000 1.5停止位0X3000
		(*(__IO uint16_t*)  RamGprsParityBitsAddr      )=0; 	   //0X0000无校验 0X0600奇校验  0X0400偶校验

		(*(__IO uint16_t*)  RamGprsDscModeAddr         )=1; 	   //GPRS 2DSC3  1自定义 通讯协议
		(*(__IO uint16_t*)  RamGprsLinkTimeAddr		     )=10; 	   // GPRS联网最长时间
		(*(__IO uint16_t*)  RamGprsLinkStopAddr	       )=2;    // 长时间没有连接上网络 停止多长时间 在重新连接
		(*(__IO uint16_t*)  RamGprsNoneReceiveTimeAddr )=1440;    //连接上网络后多长时间?没有接受到数据,开始重新连接 分钟
		(*(__IO uint16_t*)  RamGprsHeartTimeAddr       )=3000; 	   // 心跳包间隔发送时间 	S
		(*(__IO uint16_t*)  RamGprsUpdataTimeAddr      )=4000; 	   // 连续定时上传数据时间间隔

		(*(__IO uint16_t*)  RamGprsDns1DataLengthAddr  )=13;    //202.102.154.3 firstDns
		(*(__IO uint16_t*)  (RamGprsDns1DataLengthAddr+ 2))=0X3230;    //202.102.154.3 firstDns
		(*(__IO uint16_t*)  (RamGprsDns1DataLengthAddr+ 4))=0X322E;    //202.102.154.3 firstDns
		(*(__IO uint16_t*)  (RamGprsDns1DataLengthAddr+ 6))=0X3130;    //202.102.154.3 firstDns
		(*(__IO uint16_t*)  (RamGprsDns1DataLengthAddr+ 8))=0X322E;    //202.102.154.3 firstDns
		(*(__IO uint16_t*)  (RamGprsDns1DataLengthAddr+10))=0X3135;    //202.102.154.3 firstDns
		(*(__IO uint16_t*)  (RamGprsDns1DataLengthAddr+12))=0X342E;    //202.102.154.3 firstDns
		(*(__IO uint16_t*)  (RamGprsDns1DataLengthAddr+14))=0X3300;    //202.102.154.3 firstDns
		
		(*(__IO uint16_t*)  RamGprsDns2DataLengthAddr  )=13;    //202.102.152.3 secondaryDns
		(*(__IO uint16_t*)  (RamGprsDns2DataLengthAddr+ 2))=0X3230;    //202.102.152.3 secondaryDns
		(*(__IO uint16_t*)  (RamGprsDns2DataLengthAddr+ 4))=0X322E;    //202.102.152.3 secondaryDns
		(*(__IO uint16_t*)  (RamGprsDns2DataLengthAddr+ 6))=0X3130;    //202.102.152.3 secondaryDns
		(*(__IO uint16_t*)  (RamGprsDns2DataLengthAddr+ 8))=0X322E;    //202.102.152.3 secondaryDns
		(*(__IO uint16_t*)  (RamGprsDns2DataLengthAddr+10))=0X3135;    //202.102.152.3 secondaryDns
		(*(__IO uint16_t*)  (RamGprsDns2DataLengthAddr+12))=0X322E;    //202.102.152.3 secondaryDns
		(*(__IO uint16_t*)  (RamGprsDns2DataLengthAddr+14))=0X3300;    //202.102.152.3 secondaryDns
		
		(*(__IO uint16_t*)  RamGprsHeartDataLengthAddr     )=5;    //GPRS GPRS 心跳包字符长 5A 05 00 01 FE
		(*(__IO uint16_t*)  (RamGprsHeartDataLengthAddr+ 2))=0X5A05;    //GPRS GPRS 心跳包字符长
		(*(__IO uint16_t*)  (RamGprsHeartDataLengthAddr+ 6))=0X0001;    //GPRS GPRS 心跳包字符长
		(*(__IO uint16_t*)  (RamGprsHeartDataLengthAddr+ 6))=0XFE00;    //GPRS GPRS 心跳包字符长

		(*(__IO uint16_t*)  RamGprsSmidTelAddr         )=0X3133; 	   //GPRS SMID 手机号
		(*(__IO uint16_t*)  (RamGprsSmidTelAddr+ 2)    )=0X3332; 	   //GPRS SMID 手机号
		(*(__IO uint16_t*)  (RamGprsSmidTelAddr+ 4)    )=0X3633;  	   //GPRS SMID 手机号
		(*(__IO uint16_t*)  (RamGprsSmidTelAddr+ 6)    )=0X3039;  	   //GPRS SMID 手机号
		(*(__IO uint16_t*)  (RamGprsSmidTelAddr+ 8)    )=0X3337;  	   //GPRS SMID 手机号
		(*(__IO uint16_t*)  (RamGprsSmidTelAddr+10)    )=0X3900;  	   //GPRS SMID 手机号
		
		(*(__IO uint16_t*)  RamGprsAdmitTelNumberAddr  )=0X3133; 	   //授权手机号 13326309379	
		(*(__IO uint16_t*)  (RamGprsAdmitTelNumberAddr+ 2))=0X3332; 	   //授权手机号 13326309379	
		(*(__IO uint16_t*)  (RamGprsAdmitTelNumberAddr+ 4))=0X3633; 	   //授权手机号 13326309379	
		(*(__IO uint16_t*)  (RamGprsAdmitTelNumberAddr+ 6))=0X3039; 	   //授权手机号 13326309379	
		(*(__IO uint16_t*)  (RamGprsAdmitTelNumberAddr+ 8))=0X3337; 	   //授权手机号 13326309379	
		(*(__IO uint16_t*)  (RamGprsAdmitTelNumberAddr+10))=0X3900; 	   //授权手机号 13326309379	
		
		(*(__IO uint16_t*)  RamGprsSmidNumberAddr      )=0X3000;    //GPRS SMID	GPRS_SMID
    (*(__IO uint16_t*)  (RamGprsSmidNumberAddr +2) )=0X0001;    //GPRS SMID	GPRS_SMID
		
		(*(__IO uint16_t*)  RamGprsPortLengthAddr      )=4;    //GPRS IP端口号数据长度
		(*(__IO uint16_t*)  (RamGprsPortLengthAddr+ 2 ))=0X3737;    //GPRS IP端口号数据长度
		(*(__IO uint16_t*)  (RamGprsPortLengthAddr+ 4 ))=0X3737;    //GPRS IP端口号数据长度
		//接入点 APN jyzhrl.sdapn
		(*(__IO uint16_t*)  RamGprsApnLengthAddr       )=5; 	   //GPRS  接入点  CMNET UNINET
		(*(__IO uint16_t*)  (RamGprsApnLengthAddr+ 2  ))=0X434D; 	   //GPRS  接入点  CMNET UNINET
		(*(__IO uint16_t*)  (RamGprsApnLengthAddr+ 4  ))=0X4E45; 	   //GPRS  接入点  CMNET UNINET
		(*(__IO uint16_t*)  (RamGprsApnLengthAddr+ 6  ))=0X5400; 	   //GPRS  接入点  CMNET UNINET
		
		(*(__IO uint16_t*)  RamGprsHttpLengthAddr      )=15; 	   //域名 www.20cdata.com
		//WWW.
		(*(__IO uint16_t*)  (RamGprsHttpLengthAddr+ 2 ))=0X7777; 	   //GPRS  接入点  CMNET UNINET 
		(*(__IO uint16_t*)  (RamGprsHttpLengthAddr+ 4 ))=0X772E; 	   //GPRS  接入点  CMNET UNINET 
		//20CDATA.
		(*(__IO uint16_t*)  (RamGprsHttpLengthAddr+ 6 ))=0X3230; 	   //GPRS  接入点  CMNET UNINET 
		(*(__IO uint16_t*)  (RamGprsHttpLengthAddr+ 8 ))=0X6364; 	   //GPRS  接入点  CMNET UNINET 
		(*(__IO uint16_t*)  (RamGprsHttpLengthAddr+10 ))=0X6174; 	   //GPRS  接入点  CMNET UNINET 
		(*(__IO uint16_t*)  (RamGprsHttpLengthAddr+12 ))=0X612E; 	   //GPRS  接入点  CMNET UNINET 	
		//COM
		(*(__IO uint16_t*)  (RamGprsHttpLengthAddr+14 ))=0X636F; 	   //GPRS  接入点  CMNET UNINET 
		(*(__IO uint16_t*)  (RamGprsHttpLengthAddr+16 ))=0X6DFF; 	   //GPRS  接入点  CMNET UNINET 		
}
//读取Flash数据
void BspFlashRead(void )
{
	u32 flashAddress,rawAddress;
	flashAddress=FlashStartAddr;
	rawAddress=RamStartAddr;
	
	while(flashAddress < FlashEndAddr)
	{
		(*(__IO uint32_t*) rawAddress) =(*(__IO uint32_t*) flashAddress); 
		flashAddress += 4;
		rawAddress += 4;
	}

}
//写入FLSH数据  成功返回0
u8 BspFlashWrite(void )
{
	u32 flashAddress,rawAddress;
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	/* Unlock the Flash Program Erase controller */

	FLASH_Unlock();
	
	/* Clear All pending flags */
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
	
	/* Erase the FLASH pages */
	FLASHStatus = FLASH_ErasePage(FlashStartAddr);
	if(FLASH_COMPLETE!=FLASHStatus)return 1;
	
	/*  FLASH Word program of data 0x15041979 at addresses defined by StartAddr and EndAddr*/
	flashAddress=FlashStartAddr;
	rawAddress=RamStartAddr;
	
	while(flashAddress < FlashEndAddr)
	{
		FLASHStatus = FLASH_ProgramWord(flashAddress, (*(__IO uint32_t*) rawAddress));
		flashAddress += 4;
		rawAddress+=4;
		if(FLASHStatus != FLASH_COMPLETE)return 1;
	}
	
	/* Check the corectness of written data */
	flashAddress=FlashStartAddr;
	rawAddress=RamStartAddr;
	while(flashAddress < FlashEndAddr)
	{
		if((*(__IO uint32_t*) rawAddress) !=(*(__IO uint32_t*) flashAddress))return 1;
		flashAddress += 4;
		rawAddress+=4;
	}
	return 0;
}

