#ifndef _SysTick_H_
#define _SysTick_H_

#include "stm32f10x.h"
#define SysTickTime 				(SystemCoreClock/1000000-1)    /*1us,与外部晶振无关. 如果72m,那么SysTickTime = 72*/

/** @defgroup Delay_usms(u32 nsms)参数因子
  * @{
  */
#define us									*1													 /*微秒*/
#define ms									*1000												 /*毫秒*/


/** @defgroup Delay_Exported_Functions
  * @{
  */
void Delay_usms(u32 nsms);
void Delay(__IO uint32_t nTime);

#endif /*_DELAY_H_*/











/********************* BY CC *************END OF FILE**************************/
