#ifndef _TIM_H_
#define _TIM_H_

#include "head.h"//#include "TIM.h"//#include "GPIO.h"

#define Servo_ClockDivider 				    							     (SystemCoreClock/1000000-1)  /*100us 0.1ms psc*/
#define	OC1  1
#define	OC2  2
#define	OC3  4
#define	OC4  8
#endif  /*_TIM_H_*/

#define PWM_Enable(TIMx, STA)                                       TIM_Cmd(TIMx, STA;                  //使能计数器;
void PWM_Config(TIM_TypeDef * TIMx, uint16_t TIM_OCMode, uint8_t OCx, uint16_t TIM_OCPolarity, uint16_t TIM_OutputState);

void TIM2_Init    (u16 arr,u16 psc);
void TIM3_Init    (u16 arr,u16 psc);

void TIM2_PWM_Init(uint8_t OCx, u16 arr, u16 psc);
void TIM3_PWM_Init(uint8_t OCx, u16 arr, u16 psc);









/********************* BY CC *************END OF FILE**************************/
