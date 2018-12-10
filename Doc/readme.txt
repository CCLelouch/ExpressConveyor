本程序为按键检测持续，能够检测多个按键并且判断其建值，长短按，等。

调用按键程序步骤：
1：Key_GPIO_Config(void);//按键初始化函数
2：Key_Typedef Key_Scan(Key_Scan_Operate Operate);//按键扫描函数，扫描模式和读模式，扫描模式建议每5ms扫描一次，或者在main函数里面运行
3：void getKey(void);//读取按键到按键缓冲区 KeyBuffer 中,每次使用按键缓冲区 KeyBuffer 之前必须调用此函数
4：判断以下结构体
	KeyBuffer.Name == Key_Up||Key_1...;
	KeyBuffer.ReadFlag  == 1;//1表示按键跟新，在按键按下时置一，并且当调用函数getKey(void);后，读取缓冲区清空
	KeyBuffer.Active.Press == LongPress||ShortPress ;
	KeyBuffer.Active.Time 




void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2,TIM_IT_Update) !=RESET )
	{
    Key_Scan(Scan);
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
	
	
	
	