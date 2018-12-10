/**
  ******************************************************************************
  * &file    bsp_key.c
  * &author  CC
	* &previous versions V2.0.0
  * &version V3.1.0
  * &date    2018-11-14
  * &brief   
  ******************************************************************************
  * &attention
  *	使用前请先#define 	KEY16
	*					或#define		KEY4
	* Key_ReadStr.ReadFlag == 1 && Key_ReadStr.Active.Press == ShortPress 不能同时出现
	* 实现了独立按键以及4*4矩阵键盘，以及有效电平的控制
  ******************************************************************************
  */
#include "bsp_key.h"

Key_Typedef KeyBuffer;

/** &brief  KEY_GPIO_Config():用户接口函数
	*  使用前先定义#define KEY4
	*              #define KEY16
  *     &arg x...........
  * &retval None.
  */
void Key_GPIO_Config()
{
	#if ( KEYSPR==1	 )		/* 独立按键可以使用零散端口 */
    KEYSPR_GPIOCONFIG;
		GPIO_SetBits(Key_Bus,Key_Bus_L4 );
	#endif
	
	#if ( KEY16==1 )		/* 矩阵按键只能使用单一总线 */
		GPIO_Config   (Key_Bus_CLK,Key_Bus, Key_Bus_L4 , GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
		GPIO_Config   (Key_Bus_CLK,Key_Bus, Key_Bus_H4, GPIO_Speed_50MHz, GPIO_Mode_CLAMP);	
		GPIO_SetBits  (Key_Bus,	Key_Bus_L4 );
		GPIO_ResetBits(Key_Bus, Key_Bus_H4);
	#endif
}

/** &brief 矩阵按键扫描程序 KEY16_GPIODETECTION(void) 
	* &param  None
	*	&KeyTable:
          |--P4---P5----P6----P7--\
        P0|  1  |  2  |	 3  |	 4  | 
        P1|	 5  |	 6  |	 7  |	 8  |
        P2|	 9  |	10  |	11  |	12  |
        P3|	 13 |	14  |	15  | 16  |
          |-----------------------/
	*/	
Key_Num KEY16_GPIODETECTION(void)
{
  uint16_t BusBuff=0;
  Key_Num  calc=Key_Up;
	GPIO_Config   (Key_Bus_CLK,Key_Bus, Key_Bus_L4 , GPIO_Speed_50MHz, GPIO_Mode_Out_PP);/*低四位输出*/
	GPIO_Config   (Key_Bus_CLK,Key_Bus, Key_Bus_H4, GPIO_Speed_50MHz, GPIO_Mode_IPU);  /*高四位输入*/
	GPIO_SetBits  (Key_Bus,	Key_Bus_L4 );
	GPIO_ResetBits(Key_Bus, Key_Bus_H4);
	Delay_usms(1);
	BusBuff = Key_Bus->IDR & Key_Bus_H4 ;/*读高四位 读列*/	
	switch (BusBuff)
	{
	  case 0x0010: calc=Key_1;break;
		case 0x0020: calc=Key_2;break;
		case 0x0040: calc=Key_3;break;
		case 0x0080: calc=Key_4;break;
		default  : calc=Key_Up;break;
	}
	
	GPIO_Config   (Key_Bus_CLK,Key_Bus, Key_Bus_L4 , GPIO_Speed_50MHz, GPIO_Mode_IPU);/*低四位输入*/
	GPIO_Config   (Key_Bus_CLK,Key_Bus, Key_Bus_H4, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);/*高四位输出*/
	GPIO_SetBits  (Key_Bus,	Key_Bus_H4 );
	GPIO_ResetBits(Key_Bus, Key_Bus_L4);
	Delay_usms(1);
	BusBuff = Key_Bus->IDR & Key_Bus_L4;/*读低四位 读行*/
	switch (BusBuff)
	{
	  case 0x0001: calc+=0;break;
		case 0x0002: calc+=4;break;
		case 0x0004: calc+=8;break;
		case 0x0008: calc+=12;break;
		default  : calc=Key_Up;break;
	}
	return calc;
}
/** &brief getKey(void) 底层子函数
	* &param  None
	*	&returnvalue:
			|---Name------Val---------------------------------------------------------------\
			|		Key_Up		0     |                                                           |
			|		Key_1			1     |   Key_2			2     |		Key_3			3     | 	Key_4			4     |
			|		Key_5			5     |		Key_6			6     |		Key_7			7     |		Key_8			8     |
			|   Key_9			9     |		Key_10		10    |		Key_11		11    |		Key_12		12    |
			|   Key_13		13    |		Key_14		14    |		Key_15		15    |		Key_16		16    |
			|-------------------------------------------------------------------------------/
	*/
Key_Num getKeyIO(void)
{
	Key_Num return_buff = Key_Up;//初始化按键读取缓冲区
	#if ( KEYSPR==1 )		  			//完成
    KEYSPR_GPIODETECTION;     /*读取按键并返回按键关键字*/
	#endif /*KEY4*/
	#if ( KEY16==1 )
    return_buff = KEY16_GPIODETECTION();
	#endif /*KEY16*/

  return return_buff;
}

/**	
	* &brief  Key_Struct Key_Scan(Key_Scan_Operate Operate):用户接口函数s
	* &param  Operat Read||Scan  
	*	&returnvalue:Key_Struct
      |---Name------Val---------------------------------------------------------------\
      |   Key_Up    0     |                                                           |
      |   Key_1	    1     |   Key_2			2     |   Key_3			3     |   Key_4			4     |
      |   Key_5	    5     |   Key_6			6     |   Key_7			7     |   Key_8			8     |
      |   Key_9	    9     |   Key_10		10    |   Key_11		11    |   Key_12		12    |
      |   Key_13    13    |   Key_14		14    |   Key_15		15    |   Key_16		16    |
      |-------------------------------------------------------------------------------/
	* &attention:此函数需要每5-10毫秒刷新一次  
	* 实现短按，长按，消抖，扫描式检测按键弹起
	* Key16未完成
	* 按键扫描函数，扫描模式和读模式，扫描模式建议每5ms扫描一次，或者在main函数里面运行
	*/
Key_Typedef Key_Scan(Key_Scan_Operate Operate)
{
	static KeyState_TypeDef Key_State;//按键状态枚举
	static uint16_t Counter = 0,PressCounter = 0;        //扫描次数计算器
	static Key_Typedef Key_Structer;
	Key_Typedef Key_StructerBuff;
	static Key_Num ReturnVal= Key_Up;
  Key_Num Key_Buff = Key_Up;        //按键读取缓冲区
	if(Operate == Read)               //实现按键读取后将读取标志位清空
	{
		Key_StructerBuff = Key_Structer;
	  Key_Structer.ReadFlag = 0;
	  return Key_StructerBuff;
	}
	Key_Buff = (Key_Num)getKeyIO();		//每执行一次函数只读一次值
	switch(Key_State)
	{
		case Up:
			if(Key_Buff != Key_Up){  //按键按下
        Counter++;             //按键扫描计数值累加
			}
			else{                    //某段时间松开
				Counter = 0;           //如果按键弹起，记录清空  
				PressCounter++;        //按键弹起延长一下上一次键值后再将键值置高
				if(PressCounter>4)
				{
				  ReturnVal = Key_Up;
					PressCounter = 0;
				}
			}
			if(Counter>FILTERVAL){   //如果按键按下达到滤波设定值
				Counter=0;             //按键扫描计数值清空
			  Key_State = Decline;   //按键转为按下过程状态	
				Key_Structer.Active.Press = Clear;//用于确保短按后的下一次按下时是清空的
				ReturnVal = (Key_Num)Key_Buff;//捕捉到按键下降延后输出建值
			}
			break;
			
		case Decline:
			Key_State = Down;        //按键转为按下状态	
			Key_Structer.ReadFlag = 1;//当检测到下降延后标志读取位
 			break;		
			
		case Down:
			if(Key_Buff == Key_Up){  //按键弹起计数
        Counter++;
			}
			else{
			  Counter=0;
				PressCounter ++;
				if(PressCounter> 200){
				  PressCounter = 200;
				}
				if(PressCounter>=PRESSTIME){//如果按键弹按下达到长按值		 
					Key_Structer.Active.Press = LongPress;
				}	
			}
			if(Counter>FILTERVAL){   //如果按键弹起达到滤波设定值
				Counter=0;             //按键扫描计数值清空			
			  Key_State = Rising;    //按键弹起
				Key_Structer.Active.Press = Clear;//用于确保按下后检测长按清空上一次的状态
			}
			break;	
			
		case Rising:										
			Key_State = Up;          //按键弹起依旧保持上一次键值，因为短按要弹起后操作
			if(PressCounter<PRESSTIME>>1){
				Key_Structer.Active.Press = ShortPress;
			}
			PressCounter = 0;        //弹起清空
			break;
		default: break;
	}
	Key_Structer.Name = ReturnVal;
	return Key_Structer;
}

void getKey()
{
  Key_Typedef Key_Structer;
	Key_Structer = Key_Scan(Read);

	KeyBuffer.Name         = Key_Structer.Name;
	KeyBuffer.ReadFlag     = Key_Structer.ReadFlag;
	KeyBuffer.Active.Press = Key_Structer.Active.Press;
}







/********************* BY CC *************END OF FILE**************************/
