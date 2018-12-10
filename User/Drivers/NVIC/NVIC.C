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
	* ���ȼ�ֻ����λ��
	*/
void NVIC_Config(uint8_t NVIC_IRQChannel,uint8_t PreemptionPriority,uint8_t SubPriority)
{
	NVIC_InitTypeDef NVIC_InitStruct;
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 											/*����ϵͳ���ȼ����� 2�����ȼ���2�����ȼ�*/	
	NVIC_InitStruct.NVIC_IRQChannel = NVIC_IRQChannel;											/*�����ж�Դ*/
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = PreemptionPriority; /*��ռ���ȼ�*/
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = SubPriority;								/*��Ӧ���ȼ�*/
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;														/*�ж�Դ�ж�ʹ��*/
	NVIC_Init(&NVIC_InitStruct);
}










/********************* BY CC *************END OF FILE**************************/