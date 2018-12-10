/**
  ******************************************************************************
  * &file    Motor.c
  * &author  CC
  * &previous version V1.0.0 V1.1.0
  * &version  V1.2.0
  * &date     2018-05-07
  * &brief    TIMx_PWM 电机控制器.
  ******************************************************************************
  * &attention
  *	V1.0.0实现一个步进电机的启动停止，定时器计算步数
	* V1.1.0实现一个直流电机的启动停止，左右旋转
  * V1.2.0实现两个步进电机的启动停止，左右旋转,速度,步数,应用在循迹小车
  *
  ******************************************************************************
  */
#include "Motor.h"

SteppingMotor_TypeDef SteppingMotor [SteppingMotorAmount] = {0}; /*定义一个步进电机结构体数组*/
/**
  * &brief  StepperMotor01_COM_Init()
  * &param  None.
  * &retval None.
  * &attention:
  */
void StepperMotor01_COM_Init()/*端口初始化*/
{
  GPIO_Config(SteppingMotor01_ENA_GPIO_CLK,SteppingMotor01_ENA_GPIO_Port, SteppingMotor01_ENA_GPIO_Pin, GPIO_Speed_50MHz,GPIO_Mode_Out_PP);
	GPIO_Config(SteppingMotor01_DIR_GPIO_CLK,SteppingMotor01_DIR_GPIO_Port, SteppingMotor01_DIR_GPIO_Pin, GPIO_Speed_50MHz,GPIO_Mode_Out_PP);	
	SteppingMotor01_COM_ENA(1); 
	SteppingMotor01_COM_DIR(0); 
  NVIC_Config(TIM3_IRQn,3,1);/* 步进电机计数要中断函数 */
}

/**
  * &brief  StepperMotor02_COM_Init()
  * &param  None.
  * &retval None.
  * &attention:
  */
void StepperMotor02_COM_Init()/*端口初始化*/
{
  GPIO_Config(SteppingMotor02_ENA_GPIO_CLK,SteppingMotor02_ENA_GPIO_Port, SteppingMotor02_ENA_GPIO_Pin, GPIO_Speed_50MHz,GPIO_Mode_Out_PP);
	GPIO_Config(SteppingMotor02_DIR_GPIO_CLK,SteppingMotor02_DIR_GPIO_Port, SteppingMotor02_DIR_GPIO_Pin, GPIO_Speed_50MHz,GPIO_Mode_Out_PP);
	SteppingMotor02_COM_ENA(1); 
	SteppingMotor02_COM_DIR(0);
	NVIC_Config(TIM2_IRQn,2,2);/* 步进电机计数要中断函数 */  
}

/**
  * &brief  StepperMotor03_COM_Init()
  * &param  None.
  * &retval None.
  * &attention:
  */
void StepperMotor03_COM_Init()/*端口初始化*/
{
  GPIO_Config(SteppingMotor03_ENA_GPIO_CLK,SteppingMotor03_ENA_GPIO_Port, SteppingMotor03_ENA_GPIO_Pin, GPIO_Speed_50MHz,GPIO_Mode_Out_PP);
	GPIO_Config(SteppingMotor03_DIR_GPIO_CLK,SteppingMotor03_DIR_GPIO_Port, SteppingMotor03_DIR_GPIO_Pin, GPIO_Speed_50MHz,GPIO_Mode_Out_PP);
	SteppingMotor03_COM_ENA(1); 
	SteppingMotor03_COM_DIR(0); 
}

void StepperMotor_Init(uint8_t microstep,uint16_t rpm,SteppingMotor_TypeDef *SteppingMotor)
{
	float arr=0;
	/*GPIO*/
	StepperMotor01_COM_Init();
	StepperMotor02_COM_Init();
	StepperMotor03_COM_Init();
	TIM_ITConfig(SteppingMotor01_TIMx,TIM_IT_Update,ENABLE);        //使能中断源	
	for(uint8_t index=0;index<SteppingMotorAmount;index++)
	{
		SteppingMotor[index].microstep = microstep;
		SteppingMotor[index].rotation  = STOP;
		SteppingMotor[index].rpm       = rpm;
		SteppingMotor[index].mode      = CYCLE;
		SteppingMotor[index].step_loading= 1;
		SteppingMotor[index].step_current= 0;
		
		arr = 3000.0/(rpm*SteppingMotor[index].microstep)-1;   /*1prm = 1turn/60s = (200*micristep/60s)Hz 将转速装换为*/
		switch(index)
		{
			case 0:SteppingMotor01_PWM_Init(OC1, (uint16_t)arr, ClockDivider);TIM_Cmd(SteppingMotor01_TIMx, DISABLE);SteppingMotor01_TIMx->CNT=0;break;
			case 1:SteppingMotor02_PWM_Init(OC2, (uint16_t)arr, ClockDivider);TIM_Cmd(SteppingMotor02_TIMx, DISABLE);SteppingMotor02_TIMx->CNT=0;break;
			case 2:SteppingMotor03_PWM_Init(OC1, (uint16_t)arr, ClockDivider);TIM_Cmd(SteppingMotor03_TIMx, DISABLE);SteppingMotor03_TIMx->CNT=0;break;
		}
	}	
}

/**
  * &brief  StepperMotor_Upset()
  * &param  SteppingMotor_TypeDef:步进电机的结构体数组 
  * &retval None.
  * &attention:
  */
void StepperMotor_Update(SteppingMotor_TypeDef *SteppingMotor)
{
	float arr=0;
	for(uint8_t index=0;index<SteppingMotorAmount ;index++)
	{
    if(SteppingMotor[index].updateme==1)
    {
      SteppingMotor[index].updateme=0;
      arr = 3000.0/(SteppingMotor[index].rpm*SteppingMotor[index].microstep)-1;   /*1prm = 1turn/60s = (200*micristep/60s)Hz 将转速装换为周期时间*/
      switch(index)
      {
        case 0:
          switch(SteppingMotor[index].rotation){
            case STOP : SteppingMotor01_COM_ENA(1);TIM_Cmd(SteppingMotor01_TIMx,DISABLE);break;
            case BRAKE: SteppingMotor01_COM_ENA(0);TIM_Cmd(SteppingMotor01_TIMx,DISABLE);break;
            case LEFT : SteppingMotor01_COM_ENA(0);TIM_Cmd(SteppingMotor01_TIMx,ENABLE);SteppingMotor01_COM_DIR(0);break;
            case RIGHT: SteppingMotor01_COM_ENA(0);TIM_Cmd(SteppingMotor01_TIMx,ENABLE);SteppingMotor01_COM_DIR(1);break;
          }
          SteppingMotor01_PWM_Init(OC1, (uint16_t)arr, ClockDivider);
          TIM_SetCompare1(SteppingMotor01_TIMx, arr/2);
          break;
        case 1:
          switch(SteppingMotor[index].rotation){
            case STOP : SteppingMotor02_COM_ENA(1);TIM_Cmd(SteppingMotor02_TIMx,DISABLE);break;
            case BRAKE: SteppingMotor02_COM_ENA(0);TIM_Cmd(SteppingMotor02_TIMx,DISABLE);break;
            case LEFT : SteppingMotor02_COM_ENA(0);TIM_Cmd(SteppingMotor02_TIMx,ENABLE);SteppingMotor02_COM_DIR(0);break;
            case RIGHT: SteppingMotor02_COM_ENA(0);TIM_Cmd(SteppingMotor02_TIMx,ENABLE);SteppingMotor02_COM_DIR(1);break;
          }
          SteppingMotor02_PWM_Init(OC2, (uint16_t)arr, ClockDivider);
          TIM_SetCompare2(SteppingMotor02_TIMx, arr/2);
          break;
        case 2:
          switch(SteppingMotor[index].rotation){
            case STOP : SteppingMotor03_COM_ENA(1);TIM_Cmd(SteppingMotor03_TIMx,DISABLE);break;
            case BRAKE: SteppingMotor03_COM_ENA(0);TIM_Cmd(SteppingMotor03_TIMx,DISABLE);break;
            case LEFT : SteppingMotor03_COM_ENA(0);TIM_Cmd(SteppingMotor03_TIMx,ENABLE);SteppingMotor03_COM_DIR(0);break;
            case RIGHT: SteppingMotor03_COM_ENA(0);TIM_Cmd(SteppingMotor03_TIMx,ENABLE);SteppingMotor03_COM_DIR(1);break;
          }
          SteppingMotor03_PWM_Init(OC3, (uint16_t)arr, ClockDivider);
          TIM_SetCompare3(SteppingMotor03_TIMx, arr/2);
          break;
      }	    
    } 
	}	
}

/**
  * &brief  StepperMotor_Run()
  * &param  SteppingMotor_TypeDef:步进电机的结构体数组
  * &retval None.
  * &attention:不清除TIMx->CNT,只打开脉冲输出
  */
void StepperMotor_Run(SteppingMotor_TypeDef *SteppingMotor)
{	
	for(uint8_t index=0;index<SteppingMotorAmount && SteppingMotor[index].updateme==1;index++)
	{
		SteppingMotor[index].updateme=0;
		if(SteppingMotor[index].rotation==LEFT || SteppingMotor[index].rotation==RIGHT)
		{
			switch(index){
				case 0:	
					switch(SteppingMotor[index].rotation){
						case LEFT : SteppingMotor01_COM_ENA(0);TIM_Cmd(SteppingMotor01_TIMx,ENABLE);SteppingMotor01_COM_DIR(0);break;
						case RIGHT: SteppingMotor01_COM_ENA(0);TIM_Cmd(SteppingMotor01_TIMx,ENABLE);SteppingMotor01_COM_DIR(1);break;
					}break;
				case 1:
					switch(SteppingMotor[index].rotation){
						case LEFT : SteppingMotor02_COM_ENA(0);TIM_Cmd(SteppingMotor02_TIMx,ENABLE);SteppingMotor02_COM_DIR(0);break;
						case RIGHT: SteppingMotor02_COM_ENA(0);TIM_Cmd(SteppingMotor02_TIMx,ENABLE);SteppingMotor02_COM_DIR(1);break;
					}break;
				case 2:
					switch(SteppingMotor[index].rotation){
						case LEFT : SteppingMotor03_COM_ENA(0);TIM_Cmd(SteppingMotor03_TIMx,ENABLE);SteppingMotor03_COM_DIR(0);break;
						case RIGHT: SteppingMotor03_COM_ENA(0);TIM_Cmd(SteppingMotor03_TIMx,ENABLE);SteppingMotor03_COM_DIR(1);break;
					}break;
			}		
		}
	}	
}

/**
  * &brief  StepperMotor_Stop()
  * &param  SteppingMotor_TypeDef:步进电机的结构体数组
  * &retval None.
  * &attention:不清除TIMx->CNT.只关闭脉冲输出.
  */
void StepperMotor_Stop(SteppingMotor_TypeDef *SteppingMotor)
{
	for(uint8_t index=0;index<SteppingMotorAmount && SteppingMotor[index].updateme;index++)
	{
		SteppingMotor[index].updateme=0;
		if(SteppingMotor[index].rotation==STOP || SteppingMotor[index].rotation==BRAKE)
		{
			switch(index){
				case 0:
					switch(SteppingMotor[index].rotation){
						case STOP : SteppingMotor01_COM_ENA(1);TIM_Cmd(SteppingMotor01_TIMx,DISABLE);break;
						case BRAKE: SteppingMotor01_COM_ENA(0);TIM_Cmd(SteppingMotor01_TIMx,DISABLE);break;
					}break;
				case 1:
					switch(SteppingMotor[index].rotation){
						case STOP : SteppingMotor02_COM_ENA(1);TIM_Cmd(SteppingMotor02_TIMx,DISABLE);break;
						case BRAKE: SteppingMotor02_COM_ENA(0);TIM_Cmd(SteppingMotor02_TIMx,DISABLE); break;
					}break;
				case 2:
					switch(SteppingMotor[index].rotation){
						case STOP : SteppingMotor03_COM_ENA(1);TIM_Cmd(SteppingMotor03_TIMx,DISABLE);break;
						case BRAKE: SteppingMotor03_COM_ENA(0);TIM_Cmd(SteppingMotor03_TIMx,DISABLE); break;
					}break;
			}
		}	
	}		
}

/**
  * &brief  DCMotor_Update()
  * &param  DCMotor: 一个电机的参数结构体 
  * &retval None.
  * &attention:
  */
void DCMotor_Update(DCMotor_TypeDef *DCMotor)
{
  uint16_t arr;
	if(DCMotor->updateme == 1)
	{
		DCMotor->updateme = 0;
		arr = 1000000/DCMotor->frequency;
		switch(0)
		{
			case 0:DCMotor01_PWM_Init(DCMotor->OCx1|DCMotor->OCx2,arr-1,DCMotorClockDivider);break;
			case 1:/*DCMotor02_PWM_Init(OCx, arr, CD);*/break;
			case 2:/*DCMotor03_PWM_Init(OCx, arr, CD);*/break;
			default:break;
		}
		switch (DCMotor->rotation){
			case STOP :
				PWM_Config(DCMotor->TIMx, TIM_OCMode_PWM2,  DCMotor->OCx1, TIM_OCPolarity_Low, TIM_OutputState_Disable);//PWM配置   CH1
				PWM_Config(DCMotor->TIMx, TIM_OCMode_PWM2,  DCMotor->OCx2, TIM_OCPolarity_Low, TIM_OutputState_Disable);//PWM配置   CH2
				break;
			case LEFT :
				PWM_Config(DCMotor->TIMx, TIM_OCMode_PWM2,  DCMotor->OCx1, TIM_OCPolarity_Low, TIM_OutputState_Enable); //PWM配置   CH1
				PWM_Config(DCMotor->TIMx, TIM_OCMode_PWM2,  DCMotor->OCx2, TIM_OCPolarity_Low, TIM_OutputState_Disable);//PWM配置   CH2
				break;
			case RIGHT:
				PWM_Config(DCMotor->TIMx, TIM_OCMode_PWM2,  DCMotor->OCx1, TIM_OCPolarity_Low, TIM_OutputState_Disable);//PWM配置   CH1
				PWM_Config(DCMotor->TIMx, TIM_OCMode_PWM2,  DCMotor->OCx2, TIM_OCPolarity_Low, TIM_OutputState_Enable); //PWM配置   CH2
				break;
			default: break;
		}
		TIM_SetCompare1(DCMotor->TIMx, arr*DCMotor->duty/100);
		TIM_SetCompare2(DCMotor->TIMx, arr*DCMotor->duty/100);
		
		TIM_Cmd(DCMotor->TIMx,ENABLE);	                                                 /*****************************************/					
	} 
}






/****************************  USER EXAMPLE ************************************/
	
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) !=RESET )
	{
    if(SteppingMotor[0].step_loading==SteppingMotor[0].step_current)
    {
      SteppingMotor[0].step_current=0;
      SteppingMotor[0].rotation = BRAKE;
      SteppingMotor[0].updateme = 1;
      StepperMotor_Update(SteppingMotor);
    }
    if(SteppingMotor[0].mode == STEP)
      SteppingMotor[0].step_current++; 	
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update); 
	}
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) !=RESET )
	{
    if(SteppingMotor[1].step_loading==SteppingMotor[1].step_current)
    {
      SteppingMotor[1].step_current=0;
      SteppingMotor[1].rotation = BRAKE;
      SteppingMotor[1].updateme = 1;
      StepperMotor_Update(SteppingMotor);
    }
    if(SteppingMotor[1].mode == STEP)
      SteppingMotor[1].step_current++;    
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

//---------------------------------- DCMotor ----------------------------------//
//DCMotor_TypeDef DCMotor[DCMotorAmount] = {0};
//for(uint8_t index=0;index<DCMotorAmount;index++)                    /*遍历初始化*/
//{
//	DCMotor[index].duty = 50;                             /*占空比*/
//	DCMotor[index].frequency = 50;                        /*频率*/
//	DCMotor[index].GPIOx1 = GPIOB;                        /*通道GPIO*/
//	DCMotor[index].GPIOx2 = GPIOB;
//	DCMotor[index].OCx1 = OC1;
//	DCMotor[index].OCx2 = OC2;
//	DCMotor[index].Member=index;                          /*电机代码*/
//	DCMotor[index].Pin1 = GPIO_Pin_4;
//	DCMotor[index].Pin1_CLK = RCC_APB2Periph_GPIOB;
//	DCMotor[index].Pin2 = GPIO_Pin_5;
//	DCMotor[index].Pin2_CLK = RCC_APB2Periph_GPIOB;
//	DCMotor[index].TIMx = TIM3;
//	DCMotor[index].rotation = LEFT;                       /*电机方向*/
//	DCMotor->updateme = 1;
//	DCMotor_Init(&DCMotor[0]);	
//}
//DCMotor[0].rotation = STOP;
//DCMotor[0].updateme = 1;
//DCMotor[0].duty = 50;
//DCMotor[0].frequency = 50;
//DCMotor_Init(&DCMotor[0]);

/************************ BY CC *************END OF FILE***********************/
