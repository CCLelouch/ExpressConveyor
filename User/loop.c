/**
  ******************************************************************************
  * &file    loop.c
  * &author  cc
  * &version V1.0.0
  * &date    2018-04-06
  * &brief   
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */
	
#include "head.h"

void Init(void);
void Initial_Run(void);
void loop(void);




int main()
{
	Init();
	Initial_Run();
  while(1)
  {
    loop();
  }
}


/***************************************** USER CODE **********************************************/
void Init(void)//硬件资源初始化
{ 
  GPIO_Config(RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_2,GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
  USART_Config();
  DMA_UART_Config();
  Key_GPIO_Config();
  /***********************  5ms Interrupt	**************************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//对于整个中断而言的中断优先级设置只设置一次		
  StepperMotor_Init(16,60,SteppingMotor);                         /* 细分，速度rpm，步进电机结构体数组首地址 */
}

void Initial_Run(void)//软件初始化函数
{
//  USART_Send_String(USART2,"USART2\r\n");
//  DMA_USART_Send("nihao\r\n",8);
//  printf("good");
  
/***********************  PWM Init *******************************/

  SteppingMotor[0].updateme = 1;
  SteppingMotor[0].rpm = 60;
  SteppingMotor[0].mode = CYCLE;
  SteppingMotor[0].step_loading=200;/*目标步数-CYCLE模式下无效*/
  SteppingMotor[0].rotation =  STOP ;/*电机旋转方向*/
  SteppingMotor[1].updateme = 1;
  SteppingMotor[1].rpm = 20;
  SteppingMotor[1].mode = CYCLE;
  SteppingMotor[1].step_loading=200;/*目标步数-CYCLE模式下无效*/
  SteppingMotor[1].rotation =  RIGHT;/*电机旋转方向*/
  StepperMotor_Update(SteppingMotor); 

  printf("Runing\r\n"); 	
  
}

uint8_t buf[10];
enum
{
  Nun,
  Head,
  Middle,
  Rump,
  Cycle,
  Stop
}ConveyorState;

void ConveyorRuning(void);
void LED_Shine(uint8_t times);
void loop(void)				//循环主函数
{
  Delay_usms(2 ms);
  Key_Scan(Scan);
  buf[5] = 0;
  getKey();
  if(MyUSART1_Type.STA_RX&1<<15)
  {
    DMA_USART_Receive(buf,13);
    MyUSART1_Type.STA_RX = 0;
    printf((char *)buf);
    printf("\r\n");
  }
 
  if((KeyBuffer.Name == Key_1 && KeyBuffer.ReadFlag==1)||(!memcmp(buf,"acaccmdabend",12)))/*启动按钮*/
  {
    StepperMotor_Init(16,60,SteppingMotor);/* 细分，速度rpm，步进电机结构体数组首地址 */
    Delay_usms(1 ms);
    SteppingMotor[0].updateme = 1;
    SteppingMotor[0].rotation = LEFT;/*电机旋转方向*/
    SteppingMotor[0].mode= CYCLE;
    StepperMotor_Update(SteppingMotor);  
    LED_Shine(1);
    ConveyorState = Head;
    printf("Star\r\n");
  }
  else if(KeyBuffer.Name == Key_2 && KeyBuffer.ReadFlag==1||(!memcmp(buf,"acaccmdstend",12))) /*停止按钮*/         
  {
    SteppingMotor[0].updateme = 1;
    SteppingMotor[0].rotation =  STOP;/*电机旋转方向*/
    SteppingMotor[0].mode= CYCLE;
    StepperMotor_Update(SteppingMotor);
    
    ConveyorState = Stop;
    printf("Stop\r\n");
  }
  ConveyorRuning();
}

void ConveyorRuning(void)
{
  switch(ConveyorState)
  {
    case Head:
      printf("acacdataend");
      printf("\r\n");      
      if(KeyBuffer.Name == Key_3 && KeyBuffer.ReadFlag==1 )/*中间红外开关*/
      {
        SteppingMotor[0].updateme = 1;
        SteppingMotor[0].rotation =  STOP;/*电机旋转方向*/
        SteppingMotor[0].mode= CYCLE;
        StepperMotor_Update(SteppingMotor);
        printf("Scanning\r\n");
        ConveyorState = Middle;
      }
      break;
    case Middle:
        printf("acacdatbend\r\n");
//        if(MyUSART1_Type.STA_RX&1<<15)
//        {
//          DMA_USART_Receive(buf,13);
//          MyUSART1_Type.STA_RX = 0;
//          printf((char *)buf);
//          printf("\r\n");
//        }
        if(!memcmp(buf,"acaccmdacend",12))
        {
          StepperMotor_Init(16,60,SteppingMotor);/* 细分，速度rpm，步进电机结构体数组首地址 */
          Delay_usms(1 ms);
          SteppingMotor[0].updateme = 1;
          SteppingMotor[0].rotation = RIGHT;/*电机旋转方向*/
          SteppingMotor[0].mode= CYCLE;
          StepperMotor_Update(SteppingMotor);
          
          printf("\r\n");
          printf("\r\n");
          printf("\r\n");
          printf("\r\n");
        }
        if(KeyBuffer.Name == Key_4)/*末尾红外开关*/
        {
          SteppingMotor[0].updateme = 1;
          SteppingMotor[0].rotation =  STOP;/*电机旋转方向*/
          SteppingMotor[0].mode= CYCLE;
          StepperMotor_Update(SteppingMotor);
          printf("Scanning\r\n");
          ConveyorState = Rump;
        }
      break;
    case Rump:
      printf("acacdatcend");
      printf("\r\n");
      if(KeyBuffer.Name == Key_4)/*末尾红外开关*/
      {
        SteppingMotor[0].updateme = 1;
        SteppingMotor[0].rotation =  STOP;/*电机旋转方向*/
        SteppingMotor[0].mode= CYCLE;
        StepperMotor_Update(SteppingMotor);
////        printf("PlessTakeAway\r\n");
        ConveyorState = Rump;
      }
      else/*末尾红外开关*/
      {
        StepperMotor_Init(16,60,SteppingMotor);/* 细分，速度rpm，步进电机结构体数组首地址 */
        Delay_usms(1 ms);
        SteppingMotor[0].updateme = 1;
        SteppingMotor[0].rotation = LEFT;/*电机旋转方向*/
        SteppingMotor[0].mode= CYCLE;
        StepperMotor_Update(SteppingMotor);  
        ConveyorState = Head;
      }
      break;
    case Cycle:
      
      break;
    default:break;
  }
}
/***************************************** USER CODE END ******************************************/
void LED_Shine(uint8_t times)
{
  for(uint8_t t=0;t<times;t++)
  {
    PAOut(2)=0;
    Delay_usms(30 ms);
    PAOut(2)=1;
    Delay_usms(30 ms);
  }
}





/**************************** BY CC ******* END OF FILE ***********************/

