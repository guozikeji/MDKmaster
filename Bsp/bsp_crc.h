
#ifndef __BSP_CRC_H
#define __BSP_CRC_H

#include "stm32f10x_conf.h "


/* Exported macro ------------------------------------------------------------*/

/** @defgroup STM8S_EVAL_Exported_Functions
  * @{
  */

/* Includes ------------------------------------------------------------------*/
void BspCrcProcess(u8 *array,u16 *lengthPoint,u8 *control);
u8 BspCrcCheckProcess(u8 *array,u16 *lengthPoint,u8 *control);
void BspCrcReadProcess(u8 *array,u16 addr,u16 length, u16 *lengthPoint);
u8 BspCrcWriteProcess(u8 *array,u16 addr,u16 length, u16 *lengthPoint,u8 *control);
u8 BspCrcWriteSpecial(u8 *array,u16 addr,u16 length,u8 *control);
void CRC16_Array(u8 *array, u8 start_length,u16 length,u16 *crcPoint);

#endif /* __GPIO_H */

