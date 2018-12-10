/**
  ************************************************************************************
  * &file    DMA_UART.c.h
  * &author  CC
  * &version V1.0.1
  * &date    2018-12-04
  * &brief   This file as a example.
  ************************************************************************************
  * &attention
  *		
	* None
  *
  ************************************************************************************
  */
#ifndef _DMA_H_
#define _DMA_H_

#include "stm32f10x.h"
#include "USART.H"

/************************************UART*********************************************/
#define DEF_DMA_USART1             1/*Set 1 To Open DMA_USART1*/
#define DEF_DMA_USART2             0/*Set 1 To Open DMA_USART2*/
#define DEF_DMA_USART3             0/*Set 1 To Open DMA_USART3*/
#define DMA_UART_TX_BUFSIZE        64/*串口缓冲区数组大小配置*/
#define DMA_UART_RX_BUFSIZE        64/*串口缓冲区数组大小配置*/
#define DMA_UART_DMABUFSIZE        64/*DMA大小配置*/
//#define DMA_UART_SENDBUF_ADDR      UART_TX_BUF/*DMA需要的发送存储器地址*/
//#define DMA_UART_RECEIVEBUF_ADDR   UART_RX_BUF/*DMA需要的接收存储器地址*/

/*---------------------------DMA_USART1~3通道配置-用户不必修改-----------------------*/
#define DMA_UART_CLK               RCC_AHBPeriph_DMA1
#if (DEF_DMA_USART1==1)
  #define DMA_USARTx                  USART1
  #define DMA_UART_DR_ADDR           &(USART1->DR)
  #define DMA_UART_TX_CH             DMA1_Channel4/*通道4*/
  #define DMA_UART_RX_CH             DMA1_Channel5/*通道5*/
  #define DMA_UART_TX_FLAG_TC        DMA1_FLAG_TC4/*通道4的TransferComplete标志位*/
  #define DMA_UART_RX_FLAG_TC        DMA1_FLAG_TC5/*通道5的TransferComplete标志位*/
  #define DMA_UART_SENDBUF_ADDR      MyUSART1_Type.TX_Buff/*DMA需要的发送存储器地址*/
  #define DMA_UART_RECEIVEBUF_ADDR   MyUSART1_Type.RX_Buff/*DMA需要的接收存储器地址*/
#endif
#if (DEF_DMA_USART2==1)
  #define DMA_USARTx                  USART2
  #define DMA_UART_DR_ADDR           &(USART2->DR)
  #define DMA_UART_TX_CH             DMA1_Channel7/*通道7*/
  #define DMA_UART_RX_CH             DMA1_Channel6/*通道6*/
  #define DMA_UART_TX_FLAG_TC        DMA1_FLAG_TC7/*通道7的TransferComplete标志位*/
  #define DMA_UART_RX_FLAG_TC        DMA1_FLAG_TC6/*通道6的TransferComplete标志位*/
  #define DMA_UART_SENDBUF_ADDR      MyUSART2_Type.TX_Buff/*DMA需要的发送存储器地址*/
  #define DMA_UART_RECEIVEBUF_ADDR   MyUSART2_Type.RX_Buff/*DMA需要的接收存储器地址*/
#endif
#if (DEF_DMA_USART3==1)
  #define DMA_USARTx                  USART3
  #define DMA_UART_DR_ADDR           &(USART3->DR)
  #define DMA_UART_TX_CH             DMA1_Channel2/*通道2*/
  #define DMA_UART_RX_CH             DMA1_Channel3/*通道3*/
  #define DMA_UART_TX_FLAG_TC        DMA1_FLAG_TC2/*通道2的TransferComplete标志位*/
  #define DMA_UART_RX_FLAG_TC        DMA1_FLAG_TC3/*通道3的TransferComplete标志位*/
  #define DMA_UART_SENDBUF_ADDR      MyUSART3_Type.TX_Buff/*DMA需要的发送存储器地址*/
  #define DMA_UART_RECEIVEBUF_ADDR   MyUSART3_Type.RX_Buff/*DMA需要的接收存储器地址*/
#endif
/*------------------------------------------------------------------------------------*/

void DMA_UART_Config(void);
void DMA_USART_Send(uint8_t *Buff,uint8_t cound);
uint8_t* DMA_USART_Receive(uint8_t *Buff,uint8_t cound);
void DMA_USART1_IRQHandler_SubFunction(void);/*若要用DMA接收，请在对应中断内运行此函数*/
void DMA_USART2_IRQHandler_SubFunction(void);/*若要用DMA接收，请在对应中断内运行此函数*/
void DMA_USART3_IRQHandler_SubFunction(void);/*若要用DMA接收，请在对应中断内运行此函数*/

#endif  /*_DMA_H_*/

/************************* BY CC *************END OF FILE******************************/
