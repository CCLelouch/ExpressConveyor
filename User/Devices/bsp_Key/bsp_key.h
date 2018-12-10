/**
  ******************************************************************************
  * &file    bsp_key.h
  * &author  cc
  * &version V3.1.0
  * &date    2018-11-14
  * &brief   
  ******************************************************************************
  * @attention
  * 实现了低电平有效的矩阵键盘，独立按键，以及按键有效电平
  * 
  ******************************************************************************
  */
#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_
#include "stm32f10x.h"
#include "GPIO.H"
#include "delay.h"

/**************************按键宏定义修改************************/
#define FILTERVAL            2  /* 消抖值，值越大，消抖时间越长 */
#define PRESSTIME            100/* 按下时间常量，定义长按时间   */
#define ACTIVELOGIC          0  /* 0低电平有效 1高电平有效      */
/****************************************************************/

/***************按键位数选择 1 Enable 0 Disable******************/
#define KEYSPR               0         /* separate key 独立按键 */
#define	KEY16                1            /* 十六位矩阵按键按键 */
/****************************************************************/

/****** KEYSPR DEFINE 用户可自行定义多个独立按键 一般不超过16个*********/   ///////////////////*不知道为什么 PB口不行*/////////////////////
/*用户实际需要修改*/
#if ( KEYSPR==1 )
	#define Key1_Bus							GPIOC           
	#define Key2_Bus							GPIOC
	#define Key3_Bus							GPIOC
	#define Key4_Bus							GPIOC
	#define Key1_Pin							GPIO_Pin_1
	#define Key2_Pin							GPIO_Pin_2
	#define Key3_Pin							GPIO_Pin_3
	#define Key4_Pin							GPIO_Pin_4
	#define Key1_Bus_CLK					RCC_APB2Periph_GPIOC
	#define Key2_Bus_CLK					RCC_APB2Periph_GPIOC
	#define Key3_Bus_CLK					RCC_APB2Periph_GPIOC
	#define Key4_Bus_CLK					RCC_APB2Periph_GPIOC
#endif
/************************************************************************/

/************************ 判断按键有效电平 **********************/   
#if   ( ACTIVELOGIC==0 )                     /* 判断按键有效电平*/
  #define GPIO_Mode_CLAMP GPIO_Mode_IPU /* 低电平有效则上拉收入 */
#elif ( ACTIVELOGIC==1 )  
  #define GPIO_Mode_CLAMP GPIO_Mode_IPD /* 低电平有效则下拉收入 */
#endif
/****************************************************************/

/******** 独立按键初始化&独立按键检测 用户可修改独立按键的有效电平以及独立按键个数 一般不超过16个**********/
#define Read_Key(GPIOx,Pin)		GPIO_ReadInputDataBit(GPIOx,Pin)              /* 读取单个按键值用于独立按键 */
#define KEYSPR_GPIOCONFIG    {GPIO_Config(Key1_Bus_CLK,Key1_Bus,Key1_Pin,GPIO_Speed_50MHz,GPIO_Mode_CLAMP);if(ACTIVELOGIC)GPIO_ResetBits(Key1_Bus,Key1_Pin);else GPIO_SetBits(Key1_Bus,Key1_Pin);\
															GPIO_Config(Key2_Bus_CLK,Key2_Bus,Key2_Pin,GPIO_Speed_50MHz,GPIO_Mode_CLAMP);if(ACTIVELOGIC)GPIO_ResetBits(Key2_Bus,Key2_Pin);else GPIO_SetBits(Key1_Bus,Key1_Pin);\
															GPIO_Config(Key3_Bus_CLK,Key3_Bus,Key3_Pin,GPIO_Speed_50MHz,GPIO_Mode_CLAMP);if(ACTIVELOGIC)GPIO_ResetBits(Key3_Bus,Key3_Pin);else GPIO_SetBits(Key1_Bus,Key1_Pin);\
															GPIO_Config(Key4_Bus_CLK,Key4_Bus,Key4_Pin,GPIO_Speed_50MHz,GPIO_Mode_CLAMP);if(ACTIVELOGIC)GPIO_ResetBits(Key4_Bus,Key4_Pin);else GPIO_SetBits(Key1_Bus,Key1_Pin);}
															
#define KEYSPR_GPIODETECTION {     if (Read_Key(Key1_Bus,Key1_Pin) == ACTIVELOGIC) return_buff = Key_1;\
															else if	(Read_Key(Key2_Bus,Key2_Pin) == ACTIVELOGIC) return_buff = Key_2;\
															else if	(Read_Key(Key3_Bus,Key3_Pin) == ACTIVELOGIC) return_buff = Key_3;\
															else if	(Read_Key(Key4_Bus,Key4_Pin) == ACTIVELOGIC) return_buff = Key_4;\
															else 																			           return_buff = Key_Up;}
/**********************************************************************************************************/

/******* KEY16  DEFINE ************************************************************************************/
//#if ( KEY16==1 )
	#define Key_Bus						 GPIOF                /*用户实际需要修改*/
	#define Key_Bus_CLK				 RCC_APB2Periph_GPIOF /*用户实际需要修改*/
	#define Key_Bus_L4				 (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3)
	#define Key_Bus_H4				 (GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)
//#endif
/**********************************************************************************************************/

/******************** 按键相关类型****************/
typedef enum
{
	Key_Up = 0,
	Key_1 = 1,Key_2 = 2,Key_3 = 3,Key_4 = 4,
	Key_5 = 5,Key_6 = 6,Key_7 = 7,Key_8 = 8,
	Key_9 = 9,Key_10 = 10,Key_11 = 11,Key_12 = 12,
	Key_13 = 13,Key_14 = 14,Key_15 = 15,Key_16 = 16
}Key_Num;

typedef enum                 /* 按键扫描函数操作枚举 */
{
  Read = 0,        
	Scan = 1,
}Key_Scan_Operate;

typedef enum                 /* 按键状态枚举 */
{
	Up,
	Decline,
	Down,
	Rising,
}KeyState_TypeDef;
	
typedef struct{              /* 按键动作结构体 */
	enum{
		Clear = 0,	
		ShortPress = 1,
		LongPress  = 2,	
	}Press;                    /* 按键长按短按状态*/
  uint8_t Times;             /* 按键按下次数记录器 用户自行软件加一与清除*/
}KeyActive_Typedef;          /* 按键动作定义 */

typedef struct
{
  Key_Num Name;              /* 按键编号 */
	uint8_t ReadFlag :1;	     /* 用于单触发脉冲检测 */
  KeyActive_Typedef Active;  /* 按键动作*/
}Key_Typedef;                /* 按键类型   */
extern Key_Typedef KeyBuffer;/* 按键缓存器 */
/************************************************/

#endif /* _BSP_KEY_H_ */

/******************************************用户接口函数***************************************************/
void Key_GPIO_Config(void);                    /* 按键初始化函数 */
Key_Typedef Key_Scan(Key_Scan_Operate Operate);/* 按键扫描函数，扫描模式和读模式，扫描模式建议每5ms扫描一次，或者在main函数里面运行 */
void getKey(void);                             /* 读取按键到按键缓冲区 KeyBuffer 中,每次使用按键缓冲区 KeyBuffer 之前必须调用此函数 */
/*********************************************************************************************************/







/********************* BY CC *************END OF FILE**************************/
