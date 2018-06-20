/*******************************************************************************
* 本文件实现基于RTC的日期功能，提供年月日的读写。（基于ANSI-C的time.h）
* 
* 
* RTC中保存的时间格式，是UNIX时间戳格式的。即一个32bit的time_t变量（实为u32）
*
* ANSI-C的标准库中，提供了两种表示时间的数据  型：
* time_t:   	UNIX时间戳（从1970-1-1起到某时间经过的秒数）
* 	typedef unsigned int time_t;
* 
* struct tm:	Calendar格式（年月日形式）
*   tm结构如下：
*   struct tm {
*   	int tm_sec;   // 秒 seconds after the minute, 0 to 60
*   					 (0 - 60 allows for the occasional leap second)
*   	int tm_min;   // 分 minutes after the hour, 0 to 59
*		int tm_hour;  // 时 hours since midnight, 0 to 23
*		int tm_mday;  // 日 day of the month, 1 to 31
*		int tm_mon;   // 月 months since January, 0 to 11
*		int tm_year;  // 年 years since 1900
*		int tm_wday;  // 星期 days since Sunday, 0 to 6
*		int tm_yday;  // 从元旦起的天数 days since January 1, 0 to 365
* 		int tm_isdst; // 夏令时？？Daylight Savings Time flag
* 		...
* 	}
* 	其中wday，yday可以自动产生，软件直接读取
* 	mon的取值为0-11
*	***注意***：
*	tm_year:在time.h库中定义为1900年起的年份，即2008年应表示为2008-1900=108
* 	这种表示方法对用户来说不是十分友好，与现实有较大差异。
* 	所以在本文件中，屏蔽了这种差异。
* 	即外部调用本文件的函数时，tm结构体类型的日期，tm_year即为2008
* 	注意：若要调用系统库time.c中的函数，需要自行将tm_year-=1900
* 
* 成员函数说明：
* struct tm Time_ConvUnixToCalendar(time_t t);
* 	输入一个Unix时间戳（time_t），返回Calendar格式日期
* time_t Time_ConvCalendarToUnix(struct tm t);
* 	输入一个Calendar格式日期，返回Unix时间戳（time_t）
* time_t Time_GetUnixTime(void);
* 	从RTC取当前时间的Unix时间戳值
* struct tm Time_GetCalendarTime(void);
* 	从RTC取当前时间的日历时间
* void Time_SetUnixTime(time_t);
* 	输入UNIX时间戳格式时间，设置为当前RTC时间
* void Time_SetCalendarTime(struct tm t);
* 	输入Calendar格式时间，设置为当前RTC时间
* 
* 外部调用实例：
* 定义一个Calendar格式的日期变量：
* struct tm now;
* now.tm_year = 2008;
* now.tm_mon = 11;		//12月
* now.tm_mday = 20;
* now.tm_hour = 20;
* now.tm_min = 12;
* now.tm_sec = 30;
* 
* 获取当前日期时间：
* tm_now = Time_GetCalendarTime();
* 然后可以直接读tm_now.tm_wday获取星期数
* 
* 设置时间：
* Step1. tm_now.xxx = xxxxxxxxx;
* Step2. Time_SetCalendarTime(tm_now);
* 
* 计算两个时间的差
* struct tm t1,t2;
* t1_t = Time_ConvCalendarToUnix(t1);
* t2_t = Time_ConvCalendarToUnix(t2);
* dt = t1_t - t2_t;
* dt就是两个时间差的秒数
* dt_tm = mktime(dt);	//注意dt的年份匹配，ansi库中函数为相对年份，注意超限
* 另可以参考相关资料，调用ansi-c库的格式化输出等功能，ctime，strftime等
* 
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "bsp_rtc_time.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
//void Time_Set(uint32_t t);

struct datetime SETDATE={2015,8,14,15,50,0,0,0};  
struct datetime REALDATE;
time_t current_time;
struct tm time_now;
/* Private functions ---------------------------------------------------------*/
u16 const RTC_TIME_Number=0xA4A7;

u8  RTC_TIME[7];

u8  RTC_TIME_WRITE[7]={15,8,14,15,50,0,0};   

void BspRtcTimeSetUnixTime(void)
{
	BspRtcConfiguration();
	time_now.tm_year = SETDATE.year;
	time_now.tm_mon = SETDATE.month-1;			 //月0-11;
	time_now.tm_mday = SETDATE.day;
	time_now.tm_hour = SETDATE.hour;
	time_now.tm_min = SETDATE.minit;
	time_now.tm_sec = SETDATE.second;
	Time_SetCalendarTime(time_now);
	BKP_WriteBackupRegister(BKP_DR1, RTC_TIME_Number);
	BspRtcTimeGetUnixTime();
}

void BspRtcTimeGetUnixTime(void)
{
//	current_time = Time_GetUnixTime();
	time_now = Time_GetCalendarTime();
	REALDATE.year=time_now.tm_year;
	REALDATE.month=time_now.tm_mon+1;		  //月0-11;
	REALDATE.day=time_now.tm_mday;
	REALDATE.hour=time_now.tm_hour;
	REALDATE.minit=time_now.tm_min;
	REALDATE.second=time_now.tm_sec;
	REALDATE.week=time_now.tm_wday;
	REALDATE.oneDay=(u32)(REALDATE.hour)*3600+(u16)(REALDATE.minit)*60+time_now.tm_sec;
}

/*******************************************************************************
* Function Name  : BspRtcConfig
* Description    : 上电时调用本函数，自动检查是否需要RTC初始化， 
* 					若需要重新初始化RTC，则调用RTC_Configuration()完成相应操作
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BspRtcConfig(void)
{


	//我们在BKP的后备寄存器1中，存了一个特殊字符0xA5A5
	//第一次上电或后备电源掉电后，该寄存器数据丢失，
	//表明RTC数据丢失，需要重新配置
	if (BKP_ReadBackupRegister(BKP_DR1) != RTC_TIME_Number)
	{
		//重新配置RTC
		BspRtcConfiguration();
		BspRtcTimeSetUnixTime();
		//配置完成后，向后备寄存器中写特殊字符0xA5A5
		//读写寄存器前，要确定上一个操作已经结束
		BKP_WriteBackupRegister(BKP_DR1, RTC_TIME_Number);

	}
	else
	{
		//若后备寄存器没有掉电，则无需重新配置RTC
		//这里我们可以利用RCC_GetFlagStatus()函数查看本次复位类型
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
		//这是上电复位
		}
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
		//这是外部RST管脚复位
		}
		//清除RCC中复位标志
		RCC_ClearFlag();
		
		//虽然RTC模块不需要重新配置，且掉电后依靠后备电池依然运行
		//但是每次上电后，还是要使能RTCCLK???????
		//RCC_RTCCLKCmd(ENABLE);
		//等待RTC时钟与APB1时钟同步
		//RTC_WaitForSynchro();
		
		//使能秒中断
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		//等待操作完成
		RTC_WaitForLastTask();
		BspRtcTimeGetUnixTime();
	}
}



////////////////////////////////////////////////////////////////////////////////
// RTC时钟初始化！
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* Function Name  : BspRtcConfiguration
* Description    : 来重新配置RTC和BKP，仅在检测到后备寄存器数据丢失时使用
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BspRtcConfiguration(void)
{
	//启用PWR和BKP的时钟（from APB1）
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	//后备域解锁
	PWR_BackupAccessCmd(ENABLE);

	//备份寄存器模块复位
	BKP_DeInit();

	//外部32.768K其哟偶那个
	RCC_LSEConfig(RCC_LSE_ON);
	//等待稳定
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

	//RTC时钟源配置成LSE（外部32.768K）
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	//RTC开启
	RCC_RTCCLKCmd(ENABLE);

	//开启后需要等待APB1时钟与RTC时钟同步，才能读写寄存器
	RTC_WaitForSynchro();

	//读写寄存器前，要确定上一个操作已经结束
	RTC_WaitForLastTask();
	
	//使能秒中断
	RTC_ITConfig(RTC_IT_SEC, ENABLE);  

	
	//等待寄存器写入完成
	RTC_WaitForLastTask();

	//设置RTC分频器，使RTC时钟为1Hz
	//RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
	RTC_SetPrescaler(32767);

	//等待写入完成
	RTC_WaitForLastTask();

	return;
}

/*******************************************************************************
* Function Name  : Time_ConvUnixToCalendar(time_t t)
* Description    : 转换UNIX时间戳为日历时间
* Input 		 : u32 t  当前时间的UNIX时间戳
* Output		 : None
* Return		 : struct tm
*******************************************************************************/

struct tm Time_ConvUnixToCalendar(time_t t)
{
	struct tm *t_tm;
	t_tm = localtime(&t);
	t_tm->tm_year += 1900;	//localtime转换结果的tm_year是相对值，需要转成绝对值
	return *t_tm;
}

/*******************************************************************************
* Function Name  : Time_ConvCalendarToUnix(struct tm t)
* Description    : 写入RTC时钟当前时间
* Input 		 : struct tm t
* Output		 : None
* Return		 : time_t
*******************************************************************************/
time_t Time_ConvCalendarToUnix(struct tm t)
{
	t.tm_year -= 1900;  //外部tm结构体存储的年份为2008格式
						//而time.h中定义的年份格式为1900年开始的年份
						//所以，在日期转换时要考虑到这个因素。
	return mktime(&t);
}

/*******************************************************************************
* Function Name  : Time_GetUnixTime()
* Description    : 从RTC取当前时间的Unix时间戳值
* Input 		 : None
* Output		 : None
* Return		 : time_t t
*******************************************************************************/
time_t Time_GetUnixTime(void)
{
	return (time_t)RTC_GetCounter();
}

/*******************************************************************************
* Function Name  : Time_GetCalendarTime()
* Description    : 从RTC取当前时间的日历时间（struct tm）
* Input 		 : None
* Output		 : None
* Return		 : time_t t
*******************************************************************************/
struct tm Time_GetCalendarTime(void)
{
	time_t t_t;
	struct tm t_tm;
	RTC_WaitForLastTask();
	t_t = (time_t)RTC_GetCounter();
	RTC_WaitForLastTask();
	t_tm = Time_ConvUnixToCalendar(t_t);
	return t_tm;
}

/*******************************************************************************
* Function Name  : Time_SetUnixTime()
* Description    : 将给定的Unix时间戳写入RTC
* Input 		 : time_t t
* Output		 : None
* Return		 : None
*******************************************************************************/
void Time_SetUnixTime(time_t t)
{
	RTC_WaitForLastTask();
	RTC_SetCounter((uint32_t)t);
	RTC_WaitForLastTask();
	return;
}

/*******************************************************************************
* Function Name  : Time_SetCalendarTime()
* Description    : 将给定的Calendar格式时间转换成UNIX时间戳写入RTC
* Input 		 : struct tm t
* Output		 : None
* Return		 : None
*******************************************************************************/
void Time_SetCalendarTime(struct tm t)
{
	Time_SetUnixTime(Time_ConvCalendarToUnix(t));
	return;
}
