/**
  ******************************************************************************
  * &file     USART.H
  * &author   CC
  * &previous version V1.0.0
  * &version  V2.0.0
  * &date     2018-11-17
  * &brief    This file as a example.
  ******************************************************************************
  * &attention
  *
  * UART1-3
  *
  ******************************************************************************
  */
  
#ifndef _USART_H_
#define _USART_H_
#include "stm32f10x.h"
#include <stdarg.h>
#include <stdio.h>

#define UART_TX_BUFNUM                   32/*串口缓冲区数组大小*/
#define UART_RX_BUFNUM                   32/*串口缓冲区数组大小*/

/* 串口使用定义 */
#define STDIO_USART                      USART1//要用中断就不要定义这个鬼东西(不用中断可以printf)
#define DEF_USART1                       1//1开启串口，0关闭串口
#define DEF_USART2                       1//1开启串口，0关闭串口
#define DEF_USART3                       1//1开启串口，0关闭串口
#define REMAP_USER1                      0//普通(0) TX\PA9  RX\PA10;重映射(1), TX\PA6  RX\PA7; 
#define REMAP_USER2                      0//普通(0) TX\PA2  RX\PA3; 重映射(1), TX\PD5  RX\PD6; 
#define REMAP_USER3                      0//普通(0) TX\PB10 RX\PB11;重映射(1), TX\PC10 RX\PC11;全重映射(2), TX\PD8 RX\PD9;

//#define UART_DMA_OPEN                    1/*UART DMA OPEN COM Have to include DMA.H*/
/************************************ 串口1-USART1  ***********************************/
#if (DEF_USART1 == 1)
  /*----------------------------- USART GPIO 引脚宏定义 ------------------------------*/
  #if (REMAP_USER1==0)//默认端口配置
    #define USER_USART1_GPIO_CLK           RCC_APB2Periph_GPIOA//RCC_APB2Periph_GPIOB   
    #define USER_USART1_TX_GPIO_PORT       GPIOA   
    #define USER_USART1_TX_GPIO_PIN        GPIO_Pin_9
    #define USER_USART1_RX_GPIO_PORT       GPIOA
    #define USER_USART1_RX_GPIO_PIN        GPIO_Pin_10
  #elif (REMAP_USER1==1)//重映射端口配置
    #define USER_USART1_GPIO_CLK           RCC_APB2Periph_GPIOB//RCC_APB2Periph_GPIOB   
    #define USER_USART1_TX_GPIO_PORT       GPIOB
    #define USER_USART1_TX_GPIO_PIN        GPIO_Pin_6
    #define USER_USART1_RX_GPIO_PORT       GPIOB
    #define USER_USART1_RX_GPIO_PIN        GPIO_Pin_7
  #endif
  /*----------------------------------------------------------------------------------*/
  #define USER_USART1_BAUDRATE           115200/*921600*/
  #define USER_USART1_Parity             USART_Parity_No   /*USART_Parity_No|USART_Parity_Even|USART_Parity_Odd*/
  #define USER_USART1_Mode               (USART_Mode_Rx | USART_Mode_Tx)
  #define USER_USART1_IRQHandler         USART1_IRQHandler/*DMA 要加*/
#endif /* DEF_USART1 */

/************************************ 串口2-USART2  ***********************************/
#if (DEF_USART2 == 1)
  /*---------------------------- USART GPIO 引脚宏定义 -------------------------------*/
  #if (REMAP_USER2==0)//默认端口配置
  #define USER_USART2_GPIO_CLK           RCC_APB2Periph_GPIOA//RCC_APB2Periph_GPIOB   
  #define USER_USART2_TX_GPIO_PORT       GPIOA   
  #define USER_USART2_TX_GPIO_PIN        GPIO_Pin_2
  #define USER_USART2_RX_GPIO_PORT       GPIOA
  #define USER_USART2_RX_GPIO_PIN        GPIO_Pin_3
  #elif (REMAP_USER2==1)//重映射端口配置  
  #define USER_USART2_GPIO_CLK           RCC_APB2Periph_GPIOD//RCC_APB2Periph_GPIOB   
  #define USER_USART2_TX_GPIO_PORT       GPIOD   
  #define USER_USART2_TX_GPIO_PIN        GPIO_Pin_5
  #define USER_USART2_RX_GPIO_PORT       GPIOD
  #define USER_USART2_RX_GPIO_PIN        GPIO_Pin_6
  #endif
  /*----------------------------------------------------------------------------------*/
  #define USER_USART2_BAUDRATE           115200
  #define USER_USART2_Parity             USART_Parity_No   /*USART_Parity_No|USART_Parity_Even|USART_Parity_Odd*/
  #define USER_USART2_Mode               (USART_Mode_Rx | USART_Mode_Tx)
  #define USER_USART2_IRQHandler         USART2_IRQHandler
#endif /* DEF_USART2 */

/************************************ 串口3-USART3  ***********************************/
#if (DEF_USART3 == 1)
  /*---------------------------- USART GPIO 引脚宏定义 -------------------------------*/
  #if (REMAP_USER3==0)//默认端口配置
  #define USER_USART3_GPIO_CLK           RCC_APB2Periph_GPIOB//RCC_APB2Periph_GPIOB   
  #define USER_USART3_TX_GPIO_PORT       GPIOB   
  #define USER_USART3_TX_GPIO_PIN        GPIO_Pin_10
  #define USER_USART3_RX_GPIO_PORT       GPIOB
  #define USER_USART3_RX_GPIO_PIN        GPIO_Pin_11
  #elif (REMAP_USER3==1)//部分重映射端口配置  
  #define USER_USART3_GPIO_CLK           RCC_APB2Periph_GPIOC//RCC_APB2Periph_GPIOB   
  #define USER_USART3_TX_GPIO_PORT       GPIOC   
  #define USER_USART3_TX_GPIO_PIN        GPIO_Pin_10
  #define USER_USART3_RX_GPIO_PORT       GPIOC
  #define USER_USART3_RX_GPIO_PIN        GPIO_Pin_11
  #elif (REMAP_USER3==2)//全重映射端口配置 
  #define USER_USART3_GPIO_CLK           RCC_APB2Periph_GPIOD//RCC_APB2Periph_GPIOB   
  #define USER_USART3_TX_GPIO_PORT       GPIOD   
  #define USER_USART3_TX_GPIO_PIN        GPIO_Pin_8
  #define USER_USART3_RX_GPIO_PORT       GPIOD
  #define USER_USART3_RX_GPIO_PIN        GPIO_Pin_9
  #endif
  /*----------------------------------------------------------------------------------*/ 
  #define USER_USART3_BAUDRATE           115200
  #define USER_USART3_Parity             USART_Parity_No   /*USART_Parity_No|USART_Parity_Even|USART_Parity_Odd*/
  #define USER_USART3_Mode               (USART_Mode_Rx | USART_Mode_Tx)
  #define USER_USART3_IRQHandler         USART3_IRQHandler
#endif /* DEF_USART3 */

typedef struct/*此结构体用于外部可以修改硬件串口模式*/
{
	uint32_t BaudRate;//波特率      
  uint16_t Parity;                    
  uint16_t Mode;
  uint8_t TX_Buff[UART_TX_BUFNUM];
  uint8_t RX_Buff[UART_RX_BUFNUM];
  volatile uint16_t STA_RX;
  volatile uint16_t STA_TX;
}MyUSART_Type;
extern MyUSART_Type MyUSART1_Type,MyUSART2_Type,MyUSART3_Type;
extern uint8_t UART_TX_BUF[];
extern uint8_t UART_RX_BUF[];












void USART_Config(void);
void USART_Send_Byte(USART_TypeDef* USARTx, uint8_t Data);
void USART_Send_String(USART_TypeDef* USARTx, const uint8_t *String);
void printf_usart1(char* fmt,...);
void printf_usart2(char* fmt,...);
void printf_usart3(char* fmt,...);

#endif /*_USART_H_*/	

