
#ifndef __COMMON_H
#define __COMMON_H
#include "stm32f10x_conf.h "
#include "sys.h"


#define RS232_BUFFER_SIZE  512
#define RS485_BUFFER_SIZE  512

#define Start_Code     0x02 
#define Ending_Code    0x03
#define Config_Code    0x43
#define Once_Code      0x44

#define  Succeed          1
#define  Failure          0

#define  ON               1
#define  OFF              0

#define  Fire             1

#define USART1_DR_BASE           0x40013804
#define USART2_DR_BASE           0x40004404
#define USART3_DR_BASE           0x40004804

#define Sensor_In      PAin(11)
#define Step_Enable    PAout(5)
#define System_Run     PCout(13)
#define GreenLed       PEout(14)
#define RedLed         PBout(11)
#define Buzzer         PBout(10)
#define YellowLed      PEout(15)
#define SensorLed      PDout(9)
#define PC_ON          PDout(8)
u8 AsciiToHexU8(u8 high,u8 low,u8*number);
u8 AsciiToHex(u8*ascii);
u8 HexToAscii(u8 hex);
u8 AsciiCommand(u8 hex);
#endif
