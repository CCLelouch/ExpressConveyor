/**
  ******************************************************************************
  * &file    NVIC.c
  * &author  CC
  * &version V1.0.0
  * &date    2018-02-11
  * &brief   This file as a example.
  ******************************************************************************
  * &attention
  *
	* None
  *
  ******************************************************************************
  */
#include "NVIC.H"


/**
	* &brief  NVIC_Config()
	* &param  NVIC_IRQChannel:  
	*     &arg TIM2_IRQn...........
	*	&param  PreemptionPriority:
	*     &arg This parameter can be a value between 0 and 15 as described in the table
	*	&param  SubPriority:
	* 		&arg This parameter can be a value between 0 and 15 as described in the table
	*
	* 优先级只有四位。
	*/
void NVIC_Config(uint8_t NVIC_IRQChannel,uint8_t PreemptionPriority,uint8_t SubPriority)
{
	NVIC_InitTypeDef NVIC_InitStruct;
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 											/*设置系统优先级分组 2组优先级，2副优先级*/	
	NVIC_InitStruct.NVIC_IRQChannel = NVIC_IRQChannel;											/*设置中断源*/
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = PreemptionPriority; /*抢占优先级*/
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = SubPriority;								/*响应优先级*/
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;														/*中断源中断使能*/
	NVIC_Init(&NVIC_InitStruct);
}










/********************* BY CC *************END OF FILE**************************/
