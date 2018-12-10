/**
  ******************************************************************************
  * &file     USART.c
  * &author   CC
  * &previous version V1.0.0
  * &version  V2.0.0
  * &date     2018-11-17
  * &brief    
  ******************************************************************************
  * &attention
  * 目前调出USART1-3的串口发送
  * None
  * 如果用中断千万别调用 printf()
  ******************************************************************************
  */
#include "USART.H"
#include "DMA.H"
#include "GPIO.H"

uint8_t UART_TX_BUF[UART_TX_BUFNUM];/*USART的统一的收发存储器*/
uint8_t UART_RX_BUF[UART_RX_BUFNUM];/*USART的统一的收发存储器*/
MyUSART_Type MyUSART1_Type,MyUSART2_Type,MyUSART3_Type;/*USART的独立配置存储器，包括收发器*/

static void NVIC_Configuration(void)/*关于USART中断的都写在这里*/
{
  NVIC_InitTypeDef NVIC_InitStructure;
/***********If Rename USART1 Interrupt***********/
  #ifdef USER_USART1_IRQHandler
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* 嵌套向量中断控制器组选择 */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; /* 配置USART1为中断源 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; /* 抢断优先级*/ 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;/* 子优先级 */ 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; /* 使能中断 */
    NVIC_Init(&NVIC_InitStructure); /* 初始化配置NVIC */
    
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);// 使能串口空闲中断DMA
  #endif
/***********If Rename USART2 Interrupt***********/
  #ifdef USER_USART2_IRQHandler
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* 嵌套向量中断控制器组选择 */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; /* 配置USART2为中断源 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; /* 抢断优先级*/ 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;/* 子优先级 */ 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; /* 使能中断 */
    NVIC_Init(&NVIC_InitStructure); /* 初始化配置NVIC */
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);// 使能串口接收中断		  
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);// 使能串口空闲中断DMA
  #endif
/***********If Rename USART3 Interrupt***********/
  #ifdef USER_USART3_IRQHandler
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* 嵌套向量中断控制器组选择 */
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; /* 配置USART3为中断源 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; /* 抢断优先级*/ 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;/* 子优先级 */ 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; /* 使能中断 */
    NVIC_Init(&NVIC_InitStructure); /* 初始化配置NVIC */
//  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);// 使能串口接收中断			
    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);// 使能串口空闲中断DMA
  #endif
}

void USART_Config(void)
{ 
  USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef  GPIO_InitStructure;
/******************************************* USART1 *********************************************/
  #if (DEF_USART1 == 1)
    MyUSART1_Type.BaudRate = USER_USART1_BAUDRATE;
    MyUSART1_Type.Parity   = USER_USART1_Parity;
    MyUSART1_Type.Mode     = USER_USART1_Mode;
    
    RCC_APB2PeriphClockCmd(USER_USART1_GPIO_CLK, ENABLE);// 打开串口GPIO的时钟		
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//USART外设时钟 
    #if (REMAP_USER1 == 1)
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap_USART1 , ENABLE);  //USART1部分重映射
    #endif
    GPIO_InitStructure.GPIO_Pin   = USER_USART1_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;	// 将USART Tx的GPIO配置为推挽复用模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USER_USART1_TX_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = USER_USART1_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;// 将USART Rx的GPIO配置为浮空输入模式
    GPIO_Init(USER_USART1_RX_GPIO_PORT, &GPIO_InitStructure);//TX和RX不管是重映射还是默认都在一起
    
    USART_InitStruct.USART_BaudRate   = MyUSART1_Type.BaudRate;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits   = USART_StopBits_1;

    USART_InitStruct.USART_Parity     = MyUSART1_Type.Parity;
    USART_InitStruct.USART_Mode       = MyUSART1_Type.Mode;//USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStruct);
//用getchar就要关闭*************************************************************************BUG*****/ 
   
    NVIC_Configuration();// 串口中断优先级配置	DMA	
    
//#ifndef  STDIO_USART// 定义这个是用在标准输入输出库,不用于中断STDIO_USART 		
////	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);// 使能串口接收中断
//#endif    
    USART_Cmd(USART1, ENABLE);// 使能串口	
  #endif

/******************************************* USART2 *********************************************/
  #if (DEF_USART2 == 1)
    MyUSART2_Type.BaudRate = USER_USART2_BAUDRATE;
    MyUSART2_Type.Parity   = USER_USART2_Parity;
    MyUSART2_Type.Mode     = USER_USART2_Mode;
    
    RCC_APB2PeriphClockCmd(USER_USART2_GPIO_CLK, ENABLE);// 打开串口GPIO的时钟		
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//USART外设时钟 
    #if (REMAP_USER2 == 1)
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap_USART2 , ENABLE);  //USART2部分重映射
    #endif
    GPIO_InitStructure.GPIO_Pin   = USER_USART2_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;	// 将USART Tx的GPIO配置为推挽复用模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USER_USART2_TX_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = USER_USART2_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;// 将USART Rx的GPIO配置为浮空输入模式
    GPIO_Init(USER_USART2_RX_GPIO_PORT, &GPIO_InitStructure);//TX和RX不管是重映射还是默认都在一起
    
    USART_InitStruct.USART_BaudRate   = MyUSART2_Type.BaudRate;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits   = USART_StopBits_1;

    USART_InitStruct.USART_Parity     = MyUSART2_Type.Parity;
    USART_InitStruct.USART_Mode       = MyUSART2_Type.Mode;//USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &USART_InitStruct);
    
    USART_Cmd(USART2, ENABLE);// 使能串口	
  #endif
/******************************************* USART3 ******************************************/
  #if (DEF_USART3 == 1)
    MyUSART1_Type.BaudRate = USER_USART3_BAUDRATE;
    MyUSART1_Type.Parity   = USER_USART3_Parity;
    MyUSART1_Type.Mode     = USER_USART3_Mode;
    
    RCC_APB2PeriphClockCmd(USER_USART3_GPIO_CLK, ENABLE);// 打开串口GPIO的时钟		
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//USART外设时钟 
    #if (REMAP_USER3 == 1)//USART3  部分重映射
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
      GPIO_PinRemapConfig(GPIO_PartialRemap_USART3 , ENABLE);  
    #elif (REMAP_USER3 == 2)//USART3  全重映射
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
      GPIO_PinRemapConfig(GPIO_FullRemap_USART3 , ENABLE);
    #endif
    GPIO_InitStructure.GPIO_Pin   = USER_USART3_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;	// 将USART Tx的GPIO配置为推挽复用模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USER_USART3_TX_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = USER_USART3_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;// 将USART Rx的GPIO配置为浮空输入模式
    GPIO_Init(USER_USART3_RX_GPIO_PORT, &GPIO_InitStructure);//TX和RX不管是重映射还是默认都在一起

    USART_InitStruct.USART_BaudRate   = MyUSART3_Type.BaudRate;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits   = USART_StopBits_1;

    USART_InitStruct.USART_Parity     = MyUSART3_Type.Parity;
    USART_InitStruct.USART_Mode       = MyUSART3_Type.Mode;//USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3, &USART_InitStruct);
            
    USART_Cmd(USART3, ENABLE);// 使能串口	
  #endif
  NVIC_Configuration();// 串口中断优先级配置
}


/***************************************** 发送函数 *******************************************/
/** &brief  USART_Send_Byte():用户接口函数
	*
  *     &arg x...........
  * &retval None.
  */
void USART_Send_Byte(USART_TypeDef* USARTx, uint8_t Data)
{  
  USART_SendData(USARTx, Data);
  while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);//转移到移位寄存器标志位
}

/** &brief  USART_Send_String():用户接口函数
	*
  *     &arg x...........
  * &retval None.
  */
void USART_Send_String(USART_TypeDef* USARTx, const uint8_t *String)
{
  do
    USART_Send_Byte(USARTx, *String++);
  while(*String!='\0');
  while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);//发送帧结束标志位
}
/**********************************************************************************************/

/***************************************** 标准库重定向 ***************************************/
#ifdef  STDIO_USART//定义这个用在标准输入输出库,如果用中断千万别调用 printf()
  //重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
  STDIO_USART->SR &= ~USART_FLAG_TC;/*************************************/
  USART_SendData(STDIO_USART, (uint8_t) ch);/* 发送一个字节数据到串口 */		
  while (USART_GetFlagStatus(STDIO_USART, USART_FLAG_TXE) == RESET);/* 等待发送完毕 */
  return (ch);
}
//重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
  while (USART_GetFlagStatus(STDIO_USART, USART_FLAG_RXNE) == RESET);/* 等待串口输入数据 */
  USART_ClearFlag(STDIO_USART, USART_FLAG_RXNE);
  return (int)USART_ReceiveData(STDIO_USART);
}
#endif /* STDIO_USART */
/**********************************************************************************************/

/***************************************** 模拟printf  ****************************************/
void printf_usart1(char* fmt,...)/*...为参数占位符*/
{ 
  uint8_t buffer[128];
  va_list va_params; /*定义va_list类型的指针va_params，用于存储参数地址 va意为variable-argument(可变参数)*/
  va_start(va_params,fmt); //初始化va_params指针，使其指向第一个可变参数。该宏第二个参数是变参列表的前一个参数，即最后一个固定参数
  vsprintf((char *)buffer,fmt,va_params);
  va_end(va_params); //将指针va_params置为无效，结束变参的获取
  //这样，buffer里存放的既是要输出的字符串，自己编写一个串口输出字符串的函数，导进去即可
  USART_Send_String(USART1, (uint8_t *)buffer);//这样就相当于printf("%s",buffer);
}
void printf_usart2(char* fmt,...)
{ 
  uint8_t buffer[128];
  va_list va_params; 
  va_start(va_params,fmt);
  vsprintf((char *)buffer,fmt,va_params);
  va_end(va_params);
  USART_Send_String(USART2, (uint8_t *)buffer);
}
void printf_usart3(char* fmt,...)
{ 
  uint8_t buffer[128];
  va_list va_params; 
  va_start(va_params,fmt);
  vsprintf((char *)buffer,fmt,va_params);
  va_end(va_params);
  USART_Send_String(USART3, (uint8_t *)buffer);
}
/**********************************************************************************************/

/*************************** USART1 **在中断服务函数里面调用***********************************/
void USER_USART1_IRQHandler(void)
{
//  uint8_t ucTemp;
//	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
//	{		
//		ucTemp = USART_ReceiveData(USART1);
//    USART_SendData(USART1,ucTemp);    
//	}
  printf_usart1("int1\r\n");
  #if (DEF_DMA_USART1 == 1)
    DMA_USART1_IRQHandler_SubFunction();
  #endif  
}

void USER_USART2_IRQHandler(void)
{ 
  printf_usart2("int2\r\n");
  #if (DEF_DMA_USART2 == 1)
    DMA_USART2_IRQHandler_SubFunction();
  #endif  
}

void USER_USART3_IRQHandler(void)
{
  printf_usart3("int3\r\n");
  #if (DEF_DMA_USART3 == 1)
    DMA_USART3_IRQHandler_SubFunction();
  #endif  
}



/************************ BY CC *************END OF FILE***********************/
