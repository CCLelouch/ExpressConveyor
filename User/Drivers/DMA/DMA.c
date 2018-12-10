/**
  ******************************************************************************
  * &file    DMA_UART.c.c
  * &author  CC
  * &version V1.0.0
  * &date    2018-12-04
  * &brief   
  ******************************************************************************
  * &attention
  *
	* None
  *
  ******************************************************************************
  */
#include "DMA.h"
#include "USART.H"
#include <string.h>


void DMA_UART_Config()
{
/*-------------- Reset DMA init structure parameters values ------------------*/
	DMA_InitTypeDef DMA_Str; /* Initialize the DMA_PeripheralBaseAddr member */
  RCC_AHBPeriphClockCmd(DMA_UART_CLK, ENABLE);
  DMA_Str.DMA_PeripheralBaseAddr = (uint32_t)DMA_UART_DR_ADDR; /* DMA通道外设地址 */ 
  DMA_Str.DMA_MemoryBaseAddr = (uint32_t)DMA_UART_SENDBUF_ADDR;/* DMA通道存储器地址 */
  
  DMA_Str.DMA_DIR = DMA_DIR_PeripheralDST;             /* 外设作为DMA的目的端 */
  DMA_Str.DMA_BufferSize = 0;                          /* DMA缓存的大小 */
  DMA_Str.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/* 外设地址不递增 */
  DMA_Str.DMA_MemoryInc = DMA_MemoryInc_Enable;        /* 存储器地址为递增 */
  DMA_Str.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;/* 外设传输数据的单位 */
  DMA_Str.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;/* 存储器传输数据的单位（必须和上面相同）*/
  DMA_Str.DMA_Mode = DMA_Mode_Circular;                /* 工作模式 */
  DMA_Str.DMA_Priority = DMA_Priority_VeryHigh;        /* 设置其优先级 */
  DMA_Str.DMA_M2M = DMA_M2M_Disable;                   /* 关闭内存到内存的传输 */
	DMA_Init(DMA_UART_TX_CH, &DMA_Str);                  /*发送初始化*/
  DMA_Cmd(DMA_UART_TX_CH, ENABLE);
	
	DMA_Str.DMA_PeripheralBaseAddr = (uint32_t)DMA_UART_DR_ADDR; /**/
  DMA_Str.DMA_MemoryBaseAddr = (uint32_t)DMA_UART_RECEIVEBUF_ADDR ;/* Initialize the DMA_MemoryBaseAddr member */
  DMA_Str.DMA_DIR = DMA_DIR_PeripheralSRC;/* Initialize the DMA_DIR member */
  DMA_Str.DMA_BufferSize = DMA_UART_RX_BUFSIZE;/* Initialize the DMA_BufferSize member */

  DMA_Str.DMA_Priority = DMA_Priority_VeryHigh;/* Initialize the DMA_Priority member */
	DMA_Init(DMA_UART_RX_CH, &DMA_Str);//接收初始化
  DMA_Cmd(DMA_UART_RX_CH, ENABLE);	/*使能后将无法改变其寄存器值，如DMA_UART_CH->CNDTR*/
  
	USART_DMACmd(DMA_USARTx,USART_DMAReq_Tx|USART_DMAReq_Rx,ENABLE);/*收发模式*/
}

void DMA_USART_Send(uint8_t *Buff,uint8_t cound)
{
  strncpy((char *)DMA_UART_SENDBUF_ADDR, (char *)Buff, cound);
  
  DMA_Cmd(DMA_UART_TX_CH, DISABLE);/*设置传输个数前要关闭DMA*/
  DMA_UART_TX_CH->CNDTR = cound;/*设置要传输的个数，DMA会对CNDTR--*/
	DMA_Cmd(DMA_UART_TX_CH, ENABLE);/*使能后CNDTR为只读*/
  
	while(DMA_GetFlagStatus(DMA_UART_TX_FLAG_TC)==RESET);/*等待发送完毕*/
	DMA_ClearFlag(DMA_UART_TX_FLAG_TC);                  /*发送完毕后清空标志*/
  DMA_Cmd(DMA_UART_TX_CH, DISABLE);                    /*不发送则关闭DMA*/
}

uint8_t* DMA_USART_Receive(uint8_t *Buff,uint8_t cound)/**/
{
  DMA_Cmd(DMA_UART_RX_CH,DISABLE);   //关闭DMA接收，防止其间还有数据过来
  memcpy(Buff,DMA_UART_RECEIVEBUF_ADDR,cound);
  DMA_SetCurrDataCounter(DMA_UART_RX_CH,DMA_UART_RX_BUFSIZE);    //设置传输数据长度,重装填,并让接收地址偏址从0开始
  DMA_Cmd(DMA_UART_RX_CH,ENABLE);  //打开DMA接收   
	return Buff;
}

/***************************************务必在对应的串口中断内调用*********************************/
/*--------------------------------------------DMA_USART1------------------------------------------*/
void DMA_USART1_IRQHandler_SubFunction(void)
{
	if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET)/*串口空闲*/
	{	
    MyUSART1_Type.STA_RX |= 1<<15;/*接收到数据将第15位置一，表示待处理数据*/
    DMA_Cmd(DMA1_Channel5,DISABLE);   //关闭DMA接收，防止其间还有数据过来
    DMA_SetCurrDataCounter(DMA1_Channel5,DMA_UART_RX_BUFSIZE);    //设置传输数据长度,重装填,并让接收地址偏址从0开始
    DMA_Cmd(DMA1_Channel5,ENABLE);  //打开DMA接收    
    USART_ClearFlag(USART1,USART_IT_IDLE);
    (void)USART1->SR;   //先读SR，然后读DR才能清除
    (void)USART1->DR; //清USART_IT_IDLE标志
	} 
}
/*--------------------------------------------DMA_USART2------------------------------------------*/
void DMA_USART2_IRQHandler_SubFunction(void)
{
	if(USART_GetITStatus(USART2,USART_IT_IDLE)!=RESET)/*串口空闲*/
	{	
    MyUSART2_Type.STA_RX |= 1<<15;/*接收到数据将第15位置一，表示待处理数据*/
    DMA_Cmd(DMA1_Channel6,DISABLE);   //关闭DMA接收，防止其间还有数据过来
    DMA_SetCurrDataCounter(DMA1_Channel6,DMA_UART_RX_BUFSIZE);    //设置传输数据长度,重装填,并让接收地址偏址从0开始
    DMA_Cmd(DMA1_Channel6,ENABLE);  //打开DMA接收    
    USART_ClearFlag(USART2,USART_IT_IDLE);
    (void)USART2->SR;   //先读SR，然后读DR才能清除
    (void)USART2->DR; //清USART_IT_IDLE标志
	} 
}
/*--------------------------------------------DMA_USART3------------------------------------------*/
void DMA_USART3_IRQHandler_SubFunction(void)
{
	if(USART_GetITStatus(USART3,USART_IT_IDLE)!=RESET)/*串口空闲*/
	{	
    MyUSART3_Type.STA_RX |= 1<<15;/*接收到数据将第15位置一，表示待处理数据*/
    DMA_Cmd(DMA1_Channel3,DISABLE);   //关闭DMA接收，防止其间还有数据过来
    DMA_SetCurrDataCounter(DMA1_Channel3,DMA_UART_RX_BUFSIZE);    //设置传输数据长度,重装填,并让接收地址偏址从0开始
    DMA_Cmd(DMA1_Channel3,ENABLE);  //打开DMA接收    
    USART_ClearFlag(USART3,USART_IT_IDLE);
    (void)USART3->SR;   //先读SR，然后读DR才能清除
    (void)USART3->DR; //清USART_IT_IDLE标志
	} 
}



/********************* BY CC *************END OF FILE**************************/

