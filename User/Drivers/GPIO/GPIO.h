/**
  ******************************************************************************
  * &file    GPIO.h
  * &author  CC
  * &version V2.0.0
  * &date    2018-11-12
  * &brief   This file as a example.
  ******************************************************************************
  * &attention
  *			V1.0.1更新 GPIO_Config(uint16_t)参数
	* None
  *
  ******************************************************************************
  */
#ifndef _GPIO_H_
#define _GPIO_H_

#include "stm32f10x.h"

/* &DEFINE GPIO REGISTER BIT */
#define BIT_ADDR(ADDR,BIT_NO)					*(volatile unsigned int *)((ADDR & 0XF0000000)+0x02000000+((ADDR & 0X00ffffff)<<5)+(BIT_NO<<2))	
/* GPIO REGISTER BIT */

/* &DEFINE GROUP GPIO REGISTER ADDR */
#define GPIOA_ODR_ADDR						(GPIOA_BASE + 0X0C)
#define GPIOB_ODR_ADDR						(GPIOB_BASE + 0X0C)
#define GPIOC_ODR_ADDR						(GPIOC_BASE + 0X0C)
#define GPIOD_ODR_ADDR						(GPIOD_BASE + 0X0C)
#define GPIOE_ODR_ADDR						(GPIOE_BASE + 0X0C)
#define GPIOF_ODR_ADDR						(GPIOF_BASE + 0X0C)
#define GPIOG_ODR_ADDR						(GPIOG_BASE + 0X0C)

#define GPIOA_IDR_ADDR						(GPIOA_BASE + 0X08)
#define GPIOB_IDR_ADDR						(GPIOB_BASE + 0X08)
#define GPIOC_IDR_ADDR						(GPIOC_BASE + 0X08)
#define GPIOD_IDR_ADDR						(GPIOD_BASE + 0X08)
#define GPIOE_IDR_ADDR						(GPIOE_BASE + 0X08)
#define GPIOF_IDR_ADDR						(GPIOF_BASE + 0X08)
#define GPIOG_IDR_ADDR						(GPIOG_BASE + 0X08)
/* GPIO REGISTER ADDR*/

/***********************************用户接口宏定义*********************************/
/* &DEFINE GROUP GPIO OUTPUT OPERATION */
#define PAOut(N)         					BIT_ADDR(GPIOA_ODR_ADDR,N)
#define PBOut(N)         					BIT_ADDR(GPIOB_ODR_ADDR,N)
#define PCOut(N)         					BIT_ADDR(GPIOC_ODR_ADDR,N)
#define PDOut(N)         					BIT_ADDR(GPIOD_ODR_ADDR,N)
#define PEOut(N)         					BIT_ADDR(GPIOE_ODR_ADDR,N)
#define PFOut(N)         					BIT_ADDR(GPIOF_ODR_ADDR,N)
#define PGOut(N)         					BIT_ADDR(GPIOG_ODR_ADDR,N)
/* GROUP GPIO OUTPUT OPERATION */

/* &DEFINE GROUP GPIO INPUT OPERATION */
#define PAIn(N)         					BIT_ADDR(GPIOA_IDR_ADDR,N)
#define PBIn(N)         					BIT_ADDR(GPIOB_IDR_ADDR,N)
#define PCIn(N)         					BIT_ADDR(GPIOC_IDR_ADDR,N)
#define PDIn(N)         					BIT_ADDR(GPIOD_IDR_ADDR,N)
#define PEIn(N)         					BIT_ADDR(GPIOE_IDR_ADDR,N)
#define PFIn(N)         					BIT_ADDR(GPIOF_IDR_ADDR,N)
#define PGIn(N)         					BIT_ADDR(GPIOG_IDR_ADDR,N)
/* GROUP GPIO INPUT OPERATION */

/* &DEFINE GROUP GPIO CLOCK  */  /*有问题*/
//#define RCC_APB2Periph_GPIO(GPIOx)     			 if(GPIOB == GPIOx) 	       RCC_APB2Periph_GPIOA
//																			else if(GPIOB == GPIOx)     {RCC_APB2Periph_GPIOB;}
//																		else if(GPIOx == GPIOC)			RCC_APB2Periph_GPIOC;
//																		else if(GPIOx == GPIOD)			RCC_APB2Periph_GPIOD;
//																		else if(GPIOx == GPIOE)			RCC_APB2Periph_GPIOE;
//																		else if(GPIOx == GPIOF) 		RCC_APB2Periph_GPIOF;
//																		else if(GPIOx == GPIOG)			RCC_APB2Periph_GPIOG;
/* GROUP GPIO CLOCK  */

#endif  /*_GPIO_H_*/

void GPIO_Config(uint32_t RCC_APB2Periph,GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOSpeed_TypeDef GPIO_Speed,GPIOMode_TypeDef GPIO_Mode);



/********************* BY CC *************END OF FILE**************************/
