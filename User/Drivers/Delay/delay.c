/**
  ******************************************************************************
  * &file    delay.c
  * &author  cc
  * &version V1.0.0
  * &date    2018-08-29
  * &brief   
  ******************************************************************************
  * @attention
  * 次函数要配合 stm32f10x_if.h
  * 适用于CM3,CM4 基于Systic定时器的延时函数.Systic定时器在睡眠下也可以工作
  ******************************************************************************
  */
#include "delay.h"


/**
	* &brief TimingDelay延时变量，适用函数
	*	- void SysTick_Handler(),void Delay(__IO uint32_t nTime)
	*/
static __IO uint32_t TimingDelay;

/**
	*
	*/
	
	
/**
	* &brief  Delay() 
	* &param  nTimes: 循环次数
	*     &arg ns: us<2^32,
	*			&arg ms: ms<2^32/1000
	* &retval  None
	*/

void Delay_usms(u32 nTimes)
{
	SysTick->LOAD  = SysTickTime;																/*装载Systick定时器*/
	SysTick->VAL   = 0x00;																			/*清空当前寄存器*/
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |								/*配置时钟为FCLK*/
									 SysTick_CTRL_ENABLE_Msk;										/*打开定时器*/
	while(nTimes != 0)																			 	  /*循环基时次数*/
	{
	  nTimes--;
		while(!((SysTick->CTRL)&(SysTick_CTRL_COUNTFLAG_Msk)));   /*循环判断标志位*/	
	}
	SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;						    /*关闭定时器*/
}

/**
	* &brief  Delay function which using SysTic_Hander()
	* &param  nTime   The number of times 
	*     &arg
	*
	* - 使用默认配置则配置SysTick_Config(SystemCoreClock / 1000000)//systick时钟为HCLK/FCLK,
	* - 中断时间则为1us
	* - 每一次TimingDelay变化用时为Systic的计数时钟
	*/
void Delay_Init()
{
	SysTick_Config(SysTickTime);
}


/**
	* &brief  Delay function which using SysTic_Hander()
	* &param  nTime   The number of times 
	*     &arg
	*
	* - 使用默认配置则配置SysTick_Config(SystemCoreClock / 1000000)//systick时钟为HCLK/FCLK,
	* - 中断时间则为1us
	* - 每一次TimingDelay变化用时为Systic的计数时钟
	*/
//void Delay_usms(__IO uint32_t nTime)
//{
//	TimingDelay = nTime;
//	while(TimingDelay != 0x00);
//}

/**
	* &brief  Interrupt every time when Systick exception
	* &param  None
	*
	* - 使用默认配置则配置SysTick_Config(SystemCoreClock / 1000000)//systick时钟为HCLK/FCLK,
	* - 中断时间则为1ms
	* - 每一次TimingDelay变化用时为Systic的计数时钟
	* - 修改stm32f10x_it.c
	*/
void Delay_SysTick(void)
{
	if(TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}















/********************* BY CC *************END OF FILE**************************/
