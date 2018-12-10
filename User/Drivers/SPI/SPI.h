/**
  ******************************************************************************
  * &file    SPI.H
  * &author  CC
  * &version V1.0.0
  * &date    2018-08-28
  * &brief    
  ******************************************************************************
  * &attention
  *
	* None
  *
  ******************************************************************************
  */
#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f10x.h"
#include "GPIO.h"

/* SPI1  SPI2  SPI3      */
/***************  SPI模式定义   **************/
/* Mode 0 CPOL=0, CPHA=0 */ 
/* Mode 1 CPOL=0, CPHA=1 */
/* Mode 2 CPOL=1, CPHA=0 */
/* Mode 3 CPOL=1, CPHA=1 */
#define Mode0                   0
#define Mode1                   1
#define Mode2                   2
#define Mode3                   3
#define SPI_SeriesMode          Mode0
/*SPI接口定义-开头****************************/
#define SPI_Port                SPI1
#define SPI_CLK                 RCC_APB2Periph_SPI1

//CS(NSS)引脚 片选选普通GPIO即可
#define SPI_CS_CLK              RCC_APB2Periph_GPIOA  
#define SPI_CS_Port             GPIOA
#define SPI_CS_Pin              GPIO_Pin_1
//SCK引脚
#define SPI_SCK_CLK             RCC_APB2Periph_GPIOA   
#define SPI_SCK_Port             GPIOA   
#define SPI_SCK_Pin             GPIO_Pin_5
//MISO引脚
#define SPI_MISO_CLK            RCC_APB2Periph_GPIOA    
#define SPI_MISO_Port           GPIOA 
#define SPI_MISO_Pin            GPIO_Pin_6
//MOSI引脚
#define SPI_MOSI_CLK            RCC_APB2Periph_GPIOA    
#define SPI_MOSI_Port           GPIOA 
#define SPI_MOSI_Pin            GPIO_Pin_7

  #define SPI_NSS_GPIO_PORT			GPIOA
  #define SPI_NSS_GPIO_CLK			RCC_APB2Periph_GPIOA
  #define SPI_NSS_GPIO_PIN			GPIO_Pin_1 
  #define SPI_CLK_GPIO_PORT			GPIOA
  #define SPI_CLK_GPIO_CLK			RCC_APB2Periph_GPIOA
  #define SPI_CLK_GPIO_PIN			GPIO_Pin_5
  #define SPI_MISO_GPIO_PORT		GPIOA
  #define SPI_MISO_GPIO_CLK			RCC_APB2Periph_GPIOA
  #define SPI_MISO_GPIO_PIN			GPIO_Pin_6
  #define SPI_MOSI_GPIO_PORT		GPIOA
  #define SPI_MOSI_GPIO_CLK			RCC_APB2Periph_GPIOA
  #define SPI_MOSI_GPIO_PIN			GPIO_Pin_7
 


void SPI_Config(void);
uint8_t SPI_RW_Byte( uint8_t TxByte );
void SPI_RW_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length );



#endif /*_SPI_H_*/












/********************* BY CC *************END OF FILE**************************/
