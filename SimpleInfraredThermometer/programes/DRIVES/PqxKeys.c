#include "PqxKeys.h"
void initKeys(){//�����������������Ҫ����
	KeyUpSetting();
	KeyDownSetting();
	KeyOkSetting();
}
unsigned char KeysScan(){
	unsigned char keyValue = 0xff , i ; //�����ֵ
	//--��ⰴ��1--//
	if ((KeyGpio&KeyS)!= 0x07)		//��ⰴ��K1�Ƿ���
	{
//		Delay_ms(4);	//��������	
		os_wait2(K_TMO,1);    
		if (((KeyGpio&KeyS))!= 0x07)	//�ٴμ�ⰴ���Ƿ���
		{
			keyValue = (KeyGpio&KeyS);
			i = 0;
			while ((i<50) && ((KeyGpio&KeyS) != 0x07))	 //��ⰴ���Ƿ��ɿ�
			{
//				Delay_ms(5);
				os_wait2(K_TMO,1);		 
				i++;
			}
		}
	}
	return keyValue;
}
