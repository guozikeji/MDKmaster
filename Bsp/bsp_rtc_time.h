#ifndef _BSP_RTC_TIME_H_
#define _BSP_RTC_TIME_H_
#include <time.h>
#include "stm32f10x_conf.h "

struct datetime
{
		u16 year;
		u8 month;
		u8 day;
	  u8 hour;
	  u8 minit;
	  u8 second;
	  u8 week;
	  u32 oneDay;
};
extern struct tm Time_ConvUnixToCalendar(time_t t);
extern time_t Time_ConvCalendarToUnix(struct tm t);
extern time_t Time_GetUnixTime(void);
extern struct tm Time_GetCalendarTime(void);
extern void Time_SetUnixTime(time_t);
extern void Time_SetCalendarTime(struct tm t);

void BspRtcTimeGetUnixTime(void);
void BspRtcTimeSetUnixTime(void);

void BspRtcConfig(void);
void BspRtcConfiguration(void);
void BspRtcRead(uint8_t page);
uint32_t BspRtcMode(uint32_t mode);
uint32_t BspModeWrite(uint32_t mode);
#endif
