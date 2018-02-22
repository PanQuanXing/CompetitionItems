#include "PqxKeys.h"
void initKeys(){//如果外接上拉电阻就需要调用
	KeyUpSetting();
	KeyDownSetting();
	KeyOkSetting();
}
unsigned char KeysScan(){
	unsigned char keyValue = 0xff , i ; //保存键值
	//--检测按键1--//
	if ((KeyGpio&KeyS)!= 0x07)		//检测按键K1是否按下
	{
//		Delay_ms(4);	//消除抖动	
		os_wait2(K_TMO,1);    
		if (((KeyGpio&KeyS))!= 0x07)	//再次检测按键是否按下
		{
			keyValue = (KeyGpio&KeyS);
			i = 0;
			while ((i<50) && ((KeyGpio&KeyS) != 0x07))	 //检测按键是否松开
			{
//				Delay_ms(5);
				os_wait2(K_TMO,1);		 
				i++;
			}
		}
	}
	return keyValue;
}
