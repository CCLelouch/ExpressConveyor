/**
  ******************************************************************************
  * &file    SPI.c
  * &author  cc
  * &version V1.0.0
  * &date    2018-08-28
  * &brief   
  ******************************************************************************
  * @attention
  * 
  * 
  ******************************************************************************
  */
#include "SPI.H"

#define SPI_WAIT_TIMEOUT			((uint16_t)0xFFFF)

void SPI_Config()
{
  SPI_InitTypeDef		SPI_InitStructer;

  GPIO_Config(SPI_CS_CLK,SPI_CS_Port,SPI_CS_Pin,GPIO_Speed_10MHz,GPIO_Mode_Out_PP);       //Initialize  GPIO  NSS/CS 
  GPIO_Config(SPI_SCK_CLK,SPI_SCK_Port,SPI_SCK_Pin,GPIO_Speed_10MHz,GPIO_Mode_AF_PP);     //Initialize  GPIO  SCK 
  GPIO_Config(SPI_MISO_CLK,SPI_MISO_Port,SPI_MISO_Pin,GPIO_Speed_10MHz,GPIO_Mode_AF_PP);
  GPIO_Config(SPI_MOSI_CLK,SPI_MOSI_Port,SPI_MOSI_Pin,GPIO_Speed_10MHz,GPIO_Mode_AF_PP);
    
  SPI_I2S_DeInit( SPI_Port );			//复位SPI
  if( SPI1 == SPI_Port ){
		RCC_APB2PeriphClockCmd( SPI_CLK, ENABLE );			//SPI1在APB2上，打开相应SPI时钟
	}
  else{
		RCC_APB1PeriphClockCmd( SPI_CLK, ENABLE );			//SPI2 3在APB1上
	}  
  
  SPI_Cmd( SPI_Port, DISABLE );		//关闭SPI外设，配置前关闭
/*--------------- Reset SPI init structure parameters values -----------------*/
  SPI_InitStructer.SPI_Direction = SPI_Direction_2Lines_FullDuplex;/* Initialize the SPI_Direction member */
  SPI_InitStructer.SPI_Mode = SPI_Mode_Master;/* initialize the SPI_Mode member */
  SPI_InitStructer.SPI_DataSize = SPI_DataSize_8b;/* initialize the SPI_DataSize member */
  SPI_InitStructer.SPI_CPOL = SPI_CPOL_Low; /* Initialize the SPI_CPOL member */
  SPI_InitStructer.SPI_CPHA = SPI_CPHA_1Edge; /* Initialize the SPI_CPHA member */
  SPI_InitStructer.SPI_NSS = SPI_NSS_Soft;/* Initialize the SPI_NSS member */
  SPI_InitStructer.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; /* 二分频 */ 
  SPI_InitStructer.SPI_FirstBit = SPI_FirstBit_MSB; /* 高位先出 */
  SPI_InitStructer.SPI_CRCPolynomial = 7; /* CRC校验 */
  
  SPI_Init(SPI_Port, &SPI_InitStructer);
  SPI_Cmd (SPI_Port, ENABLE);
}

/**
  * @brief :SPI收发一个字节
  * @param :
  *			@TxByte: 发送的数据字节
  * @note  :非堵塞式，一旦等待超时，函数会自动退出
  * @retval:接收到的字节
  */
uint8_t SPI_RW_Byte( uint8_t TxByte )
{
	uint8_t value = 0;
	uint16_t WaitTime_buff = 0;
	
	while( RESET == SPI_I2S_GetFlagStatus( SPI_Port, SPI_I2S_FLAG_TXE ) )		//等待发送缓冲区为空
	{
		if( SPI_WAIT_TIMEOUT == ++WaitTime_buff )
		{
			break;			//如果等待超时则退出
		}
	}
	WaitTime_buff = SPI_WAIT_TIMEOUT / 2;		//重新设置接收等待时间(因为SPI的速度很快，正常情况下在发送完成之后会立即收到数据，等待时间不需要过长)
	SPI_Port->DR = TxByte;	//发送数据
	
	while( RESET == SPI_I2S_GetFlagStatus( SPI_Port, SPI_I2S_FLAG_RXNE ) )		//等待接收缓冲区非空
	{
		if( SPI_WAIT_TIMEOUT == ++WaitTime_buff )
		{
			break;			//如果等待超时则退出
		}
	}
	value = (uint8_t)SPI_Port->DR;		//读取接收数据
  
	return value;		//返回
}

/**
  * @brief :SPI收发字符串
  * @param :
  *			@ReadBuffer: 接收数据缓冲区地址
  *			@WriteBuffer:发送字节缓冲区地址
  *			@Length:字节长度
  * @note  :非堵塞式，一旦等待超时，函数会自动退出
  * @retval:无
  */
void SPI_RW_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length )
{
	GPIO_ResetBits( SPI_CS_Port, SPI_CS_Pin);			//拉低片选
	while( Length-- )
	{
		*ReadBuffer = SPI_RW_Byte( *WriteBuffer );		//收发数据
		ReadBuffer++;
		WriteBuffer++;				//读写地址加1
	}
	GPIO_SetBits( SPI_CS_Port, SPI_CS_Pin);				//拉高片选
}







/********************* BY CC *************END OF FILE**************************/
