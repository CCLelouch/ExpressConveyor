/**
  ******************************************************************************
  * &file    Motor.h
  * &author  CC
  * &version V1.2.0
  * &date    2018-04-06
  * &brief   This file as a example.
  ******************************************************************************
  * &attention
  *	
	* None
  *
  ******************************************************************************
  */
	
	
#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "stm32f10x.h"
#include "GPIO.h"
#include "TIM.h"
#include "NVIC.H"

#define ClockDivider 				    							     (SystemCoreClock/10000-1)  /*100us 0.1ms*/
#define DCMotorClockDivider 				    					 (SystemCoreClock/1000000-1)/* 1us */                    

/* STEPPING    MOTOR */
/*******************************  ROTATION DEFINE *********************************/
#define STOP                  								     0//停止
#define BRAKE                    							     1//刹车
#define LEFT                 								       2//逆时针
#define RIGHT                 								     3//顺时针
/*******************************  MODE     DEFINE *********************************/
#define CYCLE                                      0
#define STEP                                       1

/* STEPPING    MOTOR */
typedef struct
{
	uint32_t microstep :7;     /* 1-64 */
	uint32_t rotation  :2;     /* LEFT & RIGHT & STOP */
	uint32_t rpm       :12;    /* 0-4096 */
	uint32_t mode      :2;     /* CYCLE STEP */
	uint32_t           :1;
	uint32_t updateme  :2;		 /* 0 & 1 */
	uint32_t           :6;     /* NULL */
	uint32_t step_loading;
	uint32_t step_current;
}SteppingMotor_TypeDef;
/********************************  用户接口宏定义 *********************************/
#define SteppingMotorAmount   								     2										 /*最大电机个数*/
#define SteppingMotor01_ENA_GPIO_CLK    					 RCC_APB2Periph_GPIOC  /*使能端口*/
#define SteppingMotor01_ENA_GPIO_Port  						 GPIOC
#define SteppingMotor01_ENA_GPIO_Pin   						 GPIO_Pin_11
#define SteppingMotor01_DIR_GPIO_CLK    					 RCC_APB2Periph_GPIOA  /*方向端口*/
#define SteppingMotor01_DIR_GPIO_Port  						 GPIOA
#define SteppingMotor01_DIR_GPIO_Pin   						 GPIO_Pin_15

#define SteppingMotor02_ENA_GPIO_CLK    					 RCC_APB2Periph_GPIOD  /*使能端口*/
#define SteppingMotor02_ENA_GPIO_Port  						 GPIOD
#define SteppingMotor02_ENA_GPIO_Pin   						 GPIO_Pin_0
#define SteppingMotor02_DIR_GPIO_CLK    					 RCC_APB2Periph_GPIOD  /*方向端口*/
#define SteppingMotor02_DIR_GPIO_Port  						 GPIOD
#define SteppingMotor02_DIR_GPIO_Pin   						 GPIO_Pin_2

#define SteppingMotor03_ENA_GPIO_CLK    					 RCC_APB2Periph_GPIOC  /*使能端口*/
#define SteppingMotor03_ENA_GPIO_Port  						 GPIOC
#define SteppingMotor03_ENA_GPIO_Pin   						 GPIO_Pin_2
#define SteppingMotor03_DIR_GPIO_CLK    					 RCC_APB2Periph_GPIOC  /*方向端口*/
#define SteppingMotor03_DIR_GPIO_Port  						 GPIOC
#define SteppingMotor03_DIR_GPIO_Pin   						 GPIO_Pin_2

#define SteppingMotor01_COM_ENA(N) 								 PCOut(11)=(N)         /* 低电平使能 */
#define SteppingMotor01_COM_DIR(N) 								 PAOut(15)=(N)         /* 低电平左转 */
#define SteppingMotor02_COM_ENA(N) 								 PDOut(0)=(N)
#define SteppingMotor02_COM_DIR(N) 								 PDOut(2)=(N)
#define SteppingMotor03_COM_ENA(N) 								 PCOut(3)=(N)
#define SteppingMotor03_COM_DIR(N) 								 PCOut(3)=(N)

#define SteppingMotor01_TIMx                       TIM3
#define SteppingMotor02_TIMx                       TIM2
#define SteppingMotor03_TIMx                       TIM2
#define SteppingMotor01_PWM_Init(OCx, arr, CD)     TIM3_PWM_Init(OCx, arr, CD)
#define SteppingMotor02_PWM_Init(OCx, arr, CD)     TIM2_PWM_Init(OCx, arr, CD)
#define SteppingMotor03_PWM_Init(OCx, arr, CD)     TIM2_PWM_Init(OCx, arr, CD)
  
/*   DC    MOTOR   */
typedef struct
{                                                              
	uint8_t Member   :5; /* 0-16 */
	uint8_t rotation :2; /* STOP LEFT RIGHT*/
	uint8_t          :1;
	uint8_t duty     :7; /* be sure to 1-100 */
	uint8_t updateme :1; /* 0 & 1 */
	uint16_t  frequency; /* only 16-10k HZ */
	
	uint32_t      Pin1_CLK;
	uint16_t      Pin1;
  GPIO_TypeDef* GPIOx1;	
	uint32_t      Pin2_CLK;
	uint16_t      Pin2;
	GPIO_TypeDef* GPIOx2;
	
	TIM_TypeDef *  TIMx;
	uint8_t        OCx1 ;	
	uint8_t        OCx2 ;
}DCMotor_TypeDef;
extern SteppingMotor_TypeDef SteppingMotor [SteppingMotorAmount]; /*定义一个步进电机结构体数组*/
#define DCMotorAmount   								           1										 /**/
#define DCMotor01_Pin1_GPIO_CLK										 RCC_APB2Periph_GPIOB
#define DCMotor01_Pin1_GPIO_Port									 GPIOB
#define DCMotor01_Pin1   								           GPIO_Pin_4
#define DCMotor01_Pin2_GPIO_CLK										 RCC_APB2Periph_GPIOB
#define DCMotor01_Pin2_GPIO_Port									 GPIOB
#define DCMotor01_Pin2    	 						           GPIO_Pin_5

#define DCMotor01_TIMx                             TIM3
#define DCMotor01_PWM1_OCx										     OC1
#define DCMotor01_PWM2_OCx										     OC2
#define DCMotor01_PWM_Init(OCx, arr, CD)           TIM3_PWM_Init(OCx, arr, CD)
/********************************  用户函数声明  ********************************/
/* STEPPINGMOTOR_FUN */
void StepperMotor_Init  (uint8_t microstep,uint16_t rpm,SteppingMotor_TypeDef *SteppingMotor);
void StepperMotor_Update(SteppingMotor_TypeDef *SteppingMotor);
void StepperMotor_Run   (SteppingMotor_TypeDef *SteppingMotor);
void StepperMotor_Stop  (SteppingMotor_TypeDef *SteppingMotor);
/*    DCMOTOR_FUN   */
void DCMotor_Update(DCMotor_TypeDef *DCMotor);








#endif	/* _MOTOR_H_ */
/************************ BY CC *************END OF FILE***********************/
