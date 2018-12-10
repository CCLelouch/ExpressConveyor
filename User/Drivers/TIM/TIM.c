/**
  ******************************************************************************
  * &file    TIM.c
  * &author  CC
	* &previous versions V1.0.0\
  * &version V1.2.0
  * &date    2018-03-19
  * &brief   TIM2 TIM3 ALL PWM
  ******************************************************************************
  * &attention
  *
	* None
  *
  ******************************************************************************
  */

#include "TIM.h"




/*
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
*/

/**
  * &brief  TIM2_Config(u16 arr,u16 psc):
  * &param  arr:计数次数 自动重装值。 
  * &param  psc:预分频
  * &retval None.
  * &attention:当 arr=10-1,psc=7200-1时，中断时间为1ms Time = (arr+1)(psc+1)/TCLK
  */
void TIM2_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd  (RCC_APB1Periph_TIM2,ENABLE);       //打开定时器时钟线		
	TIM_TimeBaseInitStruct.TIM_Period = arr;                    //自动重装载值
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;                 //预分频
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频因子为0 ETR,TIX
	TIM_TimeBaseInit        (TIM2,&TIM_TimeBaseInitStruct);		
	TIM_ITConfig            (TIM2,TIM_IT_Update,ENABLE);        //使能中断源
	TIM_Cmd                 (TIM2,ENABLE);//使能计数器;
}

/**
  * &brief  TIM3_Config(u16 arr,u16 psc):
  * &param  arr:计数次数 自动重装值。 
  * &param  psc:预分频
  * &retval None.
  * &attention:当 arr=10-1,psc=7200-1时，中断时间为1ms Time = (arr+1)(psc+1)/TCLK
  */
void TIM3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd  (RCC_APB1Periph_TIM3,ENABLE);       //打开定时器时钟线		
	TIM_TimeBaseInitStruct.TIM_Period = arr;                    //自动重装载值
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;                 //预分频
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频因子为0 ETR,TIX 设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInit        (TIM3,&TIM_TimeBaseInitStruct);	
//	TIM_ITConfig            (TIM3,TIM_IT_Update,ENABLE);        //使能中断源	
	
//	TIM_Cmd                 (TIM3,ENABLE);//使能计数器;
}

/************************************************** PWM ***************************************************/
void PWM_Config(TIM_TypeDef * TIMx, uint16_t TIM_OCMode, uint8_t OCx, uint16_t TIM_OCPolarity, uint16_t TIM_OutputState)
{
//	TIM_Cmd                 (TIMx,DISABLE);              //计数器关闭;前面的函数是TIM3_Init(u16 arr,u16 psc)
	TIM_OCInitTypeDef TIM_OCInitStruct;                    //通道控制寄存器变量
	TIM_OCInitStruct.TIM_OCMode       = TIM_OCMode_PWM2;
  TIM_OCInitStruct.TIM_OutputState  = TIM_OutputState;
  TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStruct.TIM_Pulse        = 0x0000;
  TIM_OCInitStruct.TIM_OCPolarity   = TIM_OCPolarity;
  TIM_OCInitStruct.TIM_OCNPolarity  = TIM_OCPolarity_High;
  TIM_OCInitStruct.TIM_OCIdleState  = TIM_OCIdleState_Reset;
  TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	if(OCx&0x01 == TURE)
	{
		TIM_OC1Init             (TIMx, &TIM_OCInitStruct);
		TIM_OC1PreloadConfig    (TIMx, TIM_OCPreload_Enable);//CCMRx预重装载使能,即改变周期后下个周期才执行
	}
	if(OCx>>1&1 == TURE)
	{
		TIM_OC2Init             (TIMx, &TIM_OCInitStruct);
		TIM_OC2PreloadConfig    (TIMx, TIM_OCPreload_Enable);//CCMRx预重装载使能,即改变周期后下个周期才执行
	}
	if(OCx>>2&1 == TURE)
	{
		TIM_OC3Init             (TIMx, &TIM_OCInitStruct);
		TIM_OC3PreloadConfig    (TIMx, TIM_OCPreload_Enable);//CCMRx预重装载使能,即改变周期后下个周期才执行
	}
	if(OCx>>3&1 == TURE)
	{
		TIM_OC4Init             (TIMx, &TIM_OCInitStruct);
		TIM_OC4PreloadConfig    (TIMx, TIM_OCPreload_Enable);//CCMRx预重装载使能,即改变周期后下个周期才执行
	}

	TIM_ARRPreloadConfig    (TIMx, ENABLE);              //使能TIMx在ARR上的预装载寄存器
//z 	TIM_Cmd                 (TIMx, ENABLE);                  //使能计数器;
}

/**
  * &brief  TIM2_PWM_Init()
  * &param  uint8_t OCx, u16 arr,u16 psc:  
  *     &arg x...........
  * &retval None.
  * &attention:
  */
void TIM2_PWM_Init(uint8_t OCx, u16 arr,u16 psc)
{	
	RCC_APB2PeriphClockCmd  (RCC_APB2Periph_AFIO, ENABLE);      //AFIO特色功能寄存器时钟使能
	GPIO_PinRemapConfig     (GPIO_PartialRemap1_TIM2 , ENABLE);  //定时器TIM2部分重映射
	TIM2_Init               (arr,psc);//配置定时器
	
if(OCx   &1 == TURE)
	{
		PWM_Config            (TIM2, TIM_OCMode_PWM2,  OC1, TIM_OCPolarity_Low, TIM_OutputState_Enable);//PWM配置 CH1
	  GPIO_Config           (RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_15,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);//配置GPIO
	}
if(OCx>>1&1 == TURE)
	{
		PWM_Config            (TIM2, TIM_OCMode_PWM2,  OC2, TIM_OCPolarity_Low, TIM_OutputState_Enable);//PWM配置 CH2
		GPIO_Config           (RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_3, GPIO_Speed_50MHz,GPIO_Mode_AF_PP);//配置GPIO
	}
if(OCx>>2&1 == TURE)
	{
		PWM_Config            (TIM2, TIM_OCMode_PWM2,  OC3, TIM_OCPolarity_Low, TIM_OutputState_Enable);//PWM配置 CH3
		GPIO_Config           (RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_2, GPIO_Speed_50MHz,GPIO_Mode_AF_PP);//配置GPIO
	}
if(OCx>>3&1 == TURE)
	{
		PWM_Config            (TIM2, TIM_OCMode_PWM2,  OC4, TIM_OCPolarity_Low, TIM_OutputState_Enable);//PWM配置 CH4
		GPIO_Config           (RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_3, GPIO_Speed_50MHz,GPIO_Mode_AF_PP);//配置GPIO
	}
}

 /**
  * &brief  TIM3_PWM_Init(uint8_t OCx, u16 arr,u16 psc)
  * &param  uint8_t OCx, u16 arr,u16 psc:  
  *     &arg x...........
  * &retval None.
  * &attention:
  */
void TIM3_PWM_Init(uint8_t OCx, u16 arr,u16 psc)
{
	RCC_APB2PeriphClockCmd  (RCC_APB2Periph_AFIO, ENABLE);        //AFIO特色功能寄存器时钟使能
	GPIO_PinRemapConfig     (GPIO_Remap_SWJ_JTAGDisable , ENABLE); /*************************/

	GPIO_PinRemapConfig     (GPIO_PartialRemap_TIM3 , ENABLE);    //定时器TIM3部分重映射
	TIM3_Init								(arr,psc);													  //TIM3定时器配置

if(OCx   &1 == TURE)
	{
		PWM_Config            (TIM3, TIM_OCMode_PWM2,  OC1, TIM_OCPolarity_Low, TIM_OutputState_Enable);//PWM配置 CH1
		GPIO_Config           (RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_4,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);//配置GPIO CH1
//	GPIO_Config             (RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_6,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);//配置GPIO CH1
	}
if(OCx>>1&1 == TURE)
	{
		PWM_Config            (TIM3, TIM_OCMode_PWM2,  OC2, TIM_OCPolarity_Low, TIM_OutputState_Enable);//PWM配置 CH2
		GPIO_Config           (RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_5,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);//配置GPIO CH2
	}
if(OCx>>2&1 == TURE)
	{
		PWM_Config            (TIM3, TIM_OCMode_PWM2,  OC3, TIM_OCPolarity_Low, TIM_OutputState_Enable);//PWM配置 CH3
		GPIO_Config           (RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_0,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);//配置GPIO CH3
	}
if(OCx>>3&1 == TURE)
	{
		PWM_Config            (TIM3, TIM_OCMode_PWM2,  OC4, TIM_OCPolarity_Low, TIM_OutputState_Enable);//PWM配置 CH4
		GPIO_Config           (RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_1,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);//配置GPIO CH4
	}
}




/********************* BY CC *************END OF FILE**************************/


