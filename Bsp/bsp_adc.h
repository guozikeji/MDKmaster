

#ifndef _BSP_ADC_H
#define _BSP_ADC_H
#include "stm32f10x_conf.h "
//#include "stm32f10x.h"
//#ifdef __cplusplus
// extern "C" {
//#endif

/* Exported macro ------------------------------------------------------------*/

/** @defgroup ADC_Exported_Functions
  * @{
  */

u16 BspAdcRead(void);
void BspAdcConfig(void);
void BspAdcNvicInit(void);
//
//#ifdef __cplusplus
//}
//#endif

#endif /* __ADC_H */
