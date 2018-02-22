#include "OLED12832.h"
void OLED_Init(){
	_OLED_SCL_OUTPUT();
	_OLED_SDA_OUTPUT();
	 Delay_ms(100);
	OledSendCommand(0xAE);//关闭显示
	
	OledSendCommand(0x40);//---set low column address
	OledSendCommand(0xB0);//---set high column address

	OledSendCommand(0xC8);//-not offset

	OledSendCommand(0x81);//设置对比度
	OledSendCommand(0xff);

	OledSendCommand(0xa1);//段重定向设置

	OledSendCommand(0xa6);//
	
	OledSendCommand(0xa8);//设置驱动路数
	OledSendCommand(0x1f);
	
	OledSendCommand(0xd3);
	OledSendCommand(0x00);
	
	OledSendCommand(0xd5);
	OledSendCommand(0xf0);
	
	OledSendCommand(0xd9);
	OledSendCommand(0x22);
	
	OledSendCommand(0xda);
	OledSendCommand(0x02);
	
	OledSendCommand(0xdb);
	OledSendCommand(0x49);
	
	OledSendCommand(0x8d);
	OledSendCommand(0x14);
	
	OledSendCommand(0xaf);
	OLED_Clear();
}
void IIC_Start(){//此处改了一下顺序
	OLED_SDA=1;
	OLED_SCK=1;
	OLED_SDA=0;
	OLED_SCK=0;
}
void IIC_Stop(){
	OLED_SDA=0;
	OLED_SCK=1;
	OLED_SDA=1;
}
static bit SendByte(unsigned char iicByte){
	unsigned char i;
	OLED_SCK=0;
	for(i=0;i<8;i++)		
	{
		if(iicByte&0x80)
			OLED_SDA=1;
		else
			OLED_SDA=0;
		iicByte<<=1;
		OLED_SCK=1;
		OLED_SCK=0;
	}	
//		_SDA_INPUT();
	OLED_SCK=1;
	_nop_();_nop_();
	if(OLED_SDA)
	{
		OLED_SCK=0; 
		return 0;
	}
	OLED_SCK=0;
	return 1;
}
unsigned char OledSendCommand(unsigned char iicCommand){
	IIC_Start();
	//Slave address,SA0=0
	if(!SendByte(0x78)){
		return ADDRERROR;
	}
	//write command
	if(!SendByte(0x00)){
		return COMMANDERROR;
	}		 
	if(!SendByte(iicCommand)){
		return WCOMMANDERROR;
	}
	IIC_Stop();
	return NONEERROR;
}
unsigned char OledSendData(unsigned char iicData){
	IIC_Start();
	//Slave address,SA0=0
	if(!SendByte(0x78)){
		return ADDRERROR;
	}
	//write data
	if(!SendByte(0x40)){
		return COMMANDERROR;
	}		 
	if(!SendByte(iicData)){
		return DATAERROR;
	}
	IIC_Stop();
	return NONEERROR;
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear()  
{  
	unsigned char i,n;		    
	for(i=0;i<8;i++)
	{  
		OledSendCommand(0xb0+i);    //设置页地址（0~7）
		OledSendCommand(0x00);      //设置显示位置—列低地址
		OledSendCommand(0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
			OledSendData(0); 
	} //更新显示
}
void OLED_Full(){
	unsigned char i,n;		    
	for(i=0;i<8;i++)  
	{  
		OledSendCommand(0xb0+i);    //设置页地址（0~7）
		OledSendCommand(0x00);      //设置显示位置—列低地址
		OledSendCommand(0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
			OledSendData(1);
	}		
}
void OLED_Set_Pos(unsigned char page,unsigned char column){
	page-=1;
	column-=1;
	OledSendCommand(0xb0+page);
	OledSendCommand((column>>4)|0x10);
	OledSendCommand((column&0x0f)); 
}
void OLED_On(void){
	OledSendCommand(0X8D);  //SET DCDC命令
	OledSendCommand(0X14);  //DCDC ON
	OledSendCommand(0XAF);  //DISPLAY ON
}
void OLED_Off(void){
	OledSendCommand(0X8D);  //SET DCDC命令
	OledSendCommand(0X10);  //DCDC OFF
	OledSendCommand(0XAE);  //DISPLAY OFF
}
void Oled12832ShowByte(unsigned char page,unsigned char column,unsigned char text){
	OLED_Set_Pos(page,column);
	OledSendData(text);/*显示8x16的ASCII字到LCD上，y为页地址，x为列地址，最后为数据*/
}

void Oled12832DisplayAsciiString6x8(unsigned char page,unsigned char column,unsigned char * text,unsigned char txtlen,unsigned char charINV){
	unsigned char i,j,k;
	OLED_Set_Pos(page,column);
	for(i=0;i<txtlen;i++)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7b))    /*ASCII码字符范围32～126共95个*/
		{
			j=text[i]-0x20;     //0x20 = 32, 32为空字符，是字符表的第一个字符，此行代码计算待显示字符
				for(k=0;k<6;k++)
				{		if(charINV)	
							OledSendData(~ASCII6x8[j][k]);
						else
							OledSendData(ASCII6x8[j][k]);/*显示8x16的ASCII字到LCD上，y为页地址，x为列地址，最后为数据*/
				}
		}
	}
}

void Oled12832DisplayAsciiString8x16(unsigned char page,unsigned char column,unsigned char * text,unsigned char txtlen,unsigned char charINV){
	
	unsigned char i,j,k,n;
	for(i=0;i<txtlen;i++)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7e))    /*ASCII码字符范围32～126共95个*/
		{
			j=text[i]-0x20;     //0x20 = 32, 32为空字符，是字符表的第一个字符，此行代码计算待显示字符
							  //相对于第一个字符的位移。
			for(n=0;n<2;n++)    //需要两行显示，分上半部分和下半部分
			{
				OLED_Set_Pos(page+n,column);
				for(k=0;k<8;k++)
				{
					if(charINV)				
						OledSendData(~ASCII8x16[j][k+8*n]);/*显示8x16的ASCII字到LCD上，y为页地址，x为列地址，最后为数据*/
					else
						OledSendData(ASCII8x16[j][k+8*n]);/*显示8x16的ASCII字到LCD上，y为页地址，x为列地址，最后为数据*/
				}
			}
			column+=8;
		}
	}
}

void Oled12832ClearPage(unsigned char page,unsigned char len){
	OLED_Set_Pos(page,0);
	while(len--){
		OledSendData(0x00);
	}
}