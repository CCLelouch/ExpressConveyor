/**
  ******************************************************************************
  * &file    GPIO.c
  * &author  CC
  * &version V1.0.0
  * &date    2018-02-11
  * &brief   This file as a example.
RCC_APB2Periph_GPIOA
  ******************************************************************************
  * &attention
  *
	* None
  *
  ******************************************************************************
  */
#include "GPIO.h"

/**
	* &brief  GPIO_Config()
	* &param  RCC_APB2Periph:  
	*     &arg RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB,RCC_APB2Periph_GPIOC,
	*					 RCC_APB2Periph_GPIOD, RCC_APB2Periph_GPIOE,RCC_APB2Periph_GPIOF,
  *          RCC_APB2Periph_GPIOG,
	*	&param  GPIOx:
	*     &arg GPIOx where x can be(A-G)
	*	&param  GPIO_Pin:
	*     &arg GPIO_Pin_x where x can be(0-15) and All
	*	&param  GPIO_Speed:
	* 		&arg   GPIO_Speed_10MHz,GPIO_Speed_2MHz, GPIO_Speed_50MHz
	*	&param  GPIO_Speed:
	* 		&arg GPIO_Mode_AIN,    GPIO_Mode_IN_FLOATING,
	*					 GPIO_Mode_IPD,    GPIO_Mode_IPU,
	*					 GPIO_Mode_Out_OD, GPIO_Mode_Out_PP,
	*					 GPIO_Mode_AF_OD,  GPIO_Mode_AF_PP,
	*/	
void GPIO_Config(uint32_t RCC_APB2Periph,GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOSpeed_TypeDef GPIO_Speed,GPIOMode_TypeDef GPIO_Mode)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph ,ENABLE);
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed;
	
	GPIO_Init(GPIOx,&GPIO_InitStruct);
	GPIO_SetBits(GPIOx, GPIO_Pin);//初始值为高电平
}












/********************* BY CC *************END OF FILE**************************/

