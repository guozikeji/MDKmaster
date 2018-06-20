#ifndef _BSP_FLASH_H
#define _BSP_FLASH_H

/*预定义全局变量*/
#ifndef TASK_GPIO_GLOBALS
#define TASK_GPIO_EXT
#else
#define TASK_GPIO_EXT extern
#endif

#include "stm32f10x_conf.h "
#define FlashInitFlag	        0x1238                          //楼号高	  840 ASCII

#define FlashStartAddr          ((uint32_t)0x0803F800)			//Flash起始地址
#define FlashEndAddr            ((uint32_t)0x08040000)          //Flash结束地址
#define RamStartAddr            ((uint32_t)0x2000B800)			//raw起始地址
#define RamEndAddr      		((uint32_t)0x2000C000)          //raw结束地址

#define RamBulidingAddr	        ((uint32_t)(0x2000B800+1680))	   //楼号高	  840 ASCII
#define RamMeterAddr			((uint32_t)(0x2000B800+1684))	   //热表总数 842
#define RamUserAddr	    		((uint32_t)(0x2000B800+1686))	   //热表总数 843
#define RamCircuitChangeAddr	((uint32_t)(0x2000B800+1688))	  //线路切换时间 30S 
#define RamRunModeAddr			((uint32_t)(0x2000B800+1690))	  //运行模式 
#define RamMinValveAddr			((uint32_t)(0x2000B800+1692))	  // 防冻开度10%
#define RamMinTempRoomAddr		((uint32_t)(0x2000B800+1694))	  // 防冻温度16度
#define RamIntegralTimeAddr		((uint32_t)(0x2000B800+1696))	  // 积分时间

#define RamMbusStartAddr        ((uint32_t)(0x2000B800+1700))	   //MBUS 起始月   (11.10-3.20)
#define RamMbusEndAddr			((uint32_t)(0x2000B800+1702))	   //MBUS 起始月    (11.10-3.20)
#define RamTimeSection1Addr	  	((uint32_t)(0x2000B800+1704))	   //上传时间段1 小时  分钟
#define RamTimeSection2Addr	    ((uint32_t)(0x2000B800+1706))	   //上传时间段2 小时  分钟
#define RamTimeSection3Addr		((uint32_t)(0x2000B800+1708))	   //上传时间段3 小时  分钟
#define RamTimeSection4Addr		((uint32_t)(0x2000B800+1710))	   // 上传时间段4 小时  分钟
#define RamTimeSection5Addr		((uint32_t)(0x2000B800+1712))	   // 上传时间段5 小时  分钟
#define RamTimeSection6Addr		((uint32_t)(0x2000B800+1714))	   // 上传时间段6 小时  分钟
#define RamTimeSection7Addr		((uint32_t)(0x2000B800+1716))	   // 上传时间段7 小时  分钟
#define RamTimeSection8Addr		((uint32_t)(0x2000B800+1718))	   // 上传时间段8 小时  分钟

#define RamMbusSubpackageTimeAddr   ((uint32_t)(0x2000B800+1720))	   //MBUS 分包时间 地位
#define RamMbusBaudRateAddr		    ((uint32_t)(0x2000B800+1722))	   //MBUS 波特率
#define RamMbusDateBitsAddr	        ((uint32_t)(0x2000B800+1724))	   //MBUS 数据位 可设8-0x0000 或9-0x1000
#define RamMbusStopBitsAddr	        ((uint32_t)(0x2000B800+1726))	   //1停止位0X0000 0.5 停止位 0X1000 2停止位 0X2000 1.5停止位0X3000
#define RamMbusParityBitsAddr       ((uint32_t)(0x2000B800+1728))	   //0X0000无校验 0X0600奇校验  0X0400偶校验
#define RamMbusRestartTimeAddr	    ((uint32_t)(0x2000B800+1730))	   // MBUS 重启时间 5s   
#define RamMbusCircuitTimeAddr	    ((uint32_t)(0x2000B800+1732))	   // MBUS 循环时间 2S
#define RamMbusErrorCountAddr		((uint32_t)(0x2000B800+1734))	   // MBUS 读取错误次数	4

#define RamLcdControlAddr           ((uint32_t)(0x2000B800+1740))	   //0本地控制，1远程控制
#define RamLcdRefreshTimeAddr		((uint32_t)(0x2000B800+1742))	   //LCD刷屏时间 	 250
#define RamLcdStartTimeAddr	        ((uint32_t)(0x2000B800+1744))	   // LCD显示开始时间 7:30开始显示
#define RamLcdEndTimeAddr	        ((uint32_t)(0x2000B800+1746))	   // LCD显示结束时间 18:30结束显示
#define RamLcdHeadFrontColorAddr    ((uint32_t)(0x2000B800+1748))	   //LCD头文件前景色 灰色
#define RamLcdHeadBackColorAddr	    ((uint32_t)(0x2000B800+1750))	   //LCD头文件背景色 红色
#define RamLcdContentFrontColorAddr	((uint32_t)(0x2000B800+1752))	   //LCD内容前景色 灰色
#define RamLcdContentBackColorAddr  ((uint32_t)(0x2000B800+1754))	   //LCD内容背景色	洋红
#define RamLcdHighLightColorAddr	((uint32_t)(0x2000B800+1752))	   //LCD高亮显示色	蓝色
#define RamLcdBrightnessAddr        ((uint32_t)(0x2000B800+1754))	   //LCD高亮 比例 0灭 1000最大

#define RamRs485SubpackageTimeAddr  ((uint32_t)(0x2000B800+1760))	   //RS485分包时间
#define RamRs485BaudRateAddr		((uint32_t)(0x2000B800+1762))	   // 波特率
#define RamRs485DateBitsAddr	    ((uint32_t)(0x2000B800+1764))	   // 数据位 可设8-0x0000 或9-0x1000
#define RamRs485StopBitsAddr	    ((uint32_t)(0x2000B800+1766))	   //1停止位0X0000 0.5 停止位 0X1000 2停止位 0X2000 1.5停止位0X3000
#define RamRs485ParityBitsAddr      ((uint32_t)(0x2000B800+1768))	   //0X0000无校验 0X0600奇校验  0X0400偶校验

#define RamGprsSubpackageTimeAddr   ((uint32_t)(0x2000B800+1770))	   //GPRS分包时间
#define RamGprsBaudRateAddr		    ((uint32_t)(0x2000B800+1772))	   // 波特率
#define RamGprsDateBitsAddr	        ((uint32_t)(0x2000B800+1774))	   // 数据位 可设8-0x0000 或9-0x1000
#define RamGprsStopBitsAddr	        ((uint32_t)(0x2000B800+1776))	   //1停止位0X0000 0.5 停止位 0X1000 2停止位 0X2000 1.5停止位0X3000
#define RamGprsParityBitsAddr       ((uint32_t)(0x2000B800+1778))	   //0X0000无校验 0X0600奇校验  0X0400偶校验

#define RamGprsDscModeAddr          ((uint32_t)(0x2000B800+1780))	   //GPRS 2DSC3  1自定义 通讯协议
#define RamGprsLinkTimeAddr		    ((uint32_t)(0x2000B800+1782))	   // GPRS联网最长时间
#define RamGprsLinkStopAddr	        ((uint32_t)(0x2000B800+1784))	   // 最长时间没有连接上网络 停止多长时间 在重新连接
#define RamGprsNoneReceiveTimeAddr	((uint32_t)(0x2000B800+1786))	   //连接上网络后多长时间?没有接受到数据,开始重新连接
#define RamGprsHeartTimeAddr        ((uint32_t)(0x2000B800+1788))	   // 心跳包间隔发送时间	
#define RamGprsUpdataTimeAddr       ((uint32_t)(0x2000B800+1790))	   // 连续定时上传数据时间间隔

#define RamGprsDns1DataLengthAddr       ((uint32_t)(0x2000B800+1800))	   //202.102.152.3 firstDns
#define RamGprsDns2DataLengthAddr       ((uint32_t)(0x2000B800+1820))	   //202.102.152.3 secondaryDns
#define RamGprsHeartDataLengthAddr      ((uint32_t)(0x2000B800+1840))	   //GPRS GPRS 心跳包字符长

#define RamGprsSmidTelAddr              ((uint32_t)(0x2000B800+1860))	   //GPRS SMID 手机号
#define RamGprsAdmitTelNumberAddr       ((uint32_t)(0x2000B800+1872))	   //授权手机号 13326309379	
#define RamGprsSmidNumberAddr           ((uint32_t)(0x2000B800+1884))	   //GPRS SMID	GPRS_SMID

#define RamGprsPortLengthAddr           ((uint32_t)(0x2000B800+1888))	   //GPRS IP端口号数据长度
//接入点 APN jyzhrl.sdapn
#define RamGprsApnLengthAddr            ((uint32_t)(0x2000B800+1900))	   //GPRS  接入点  CMNET UNINET

#define RamGprsHttpLengthAddr           ((uint32_t)(0x2000B800+1940))	   //GPRS  域名 www.20cdata.com


void BspRawInit(void);
u8 BspFlashInit(void);
void BspFlashRead(void );
u8 BspFlashWrite(void );

#endif 
