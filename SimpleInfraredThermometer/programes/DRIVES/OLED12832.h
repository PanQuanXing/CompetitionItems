#ifndef _OLED12832_H_
#define _OLED12832_H_
#include "STC15F2K60S2.h"
#include "PqxDelay.h"
#include "intrins.h"
#include "PqxFont.h"
#define	OLED_SCK	P24		//时钟
#define OLED_SDA	P25		//数据
#define		_OLED_SDA_OUTPUT() 	P2M1 |= 0x20;P2M0 |=0x20
#define		_OLED_SCL_OUTPUT() 	P2M1 |= 0x10;P2M0 |=0x10
//配置MLX90614的IO
//#define		_SDA_OUTPUT() 	P2M1 |= 0x80;P2M0 |=0x80
//#define		_SDA_INPUT()  	P2M1 |= 0x80;P2M0 &=~0x80
//#define		_SCL_OUTPUT() 	P2M1 |= 0x40;P2M0 |=0x40
#define ADDRERROR	0x01
#define COMMANDERROR 0x02
#define WCOMMANDERROR 0x04
#define DATAERROR 0x08
#define NONEERROR 0x00
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Full(void);
void OLED_On(void);
void OLED_Off(void);
void IIC_Start(void);
void IIC_Stop(void);
static bit SendByte(unsigned char iicByte);//iic通讯最重要的函数
unsigned char OledSendCommand(unsigned char iicCommand);//通过调用SendByte
unsigned char OledSendData(unsigned char iicData);//通过调用SendByte
void Oled12832ShowByte(unsigned char page,unsigned char column,unsigned char text);
void Oled12832DisplayAsciiString6x8(unsigned char page,unsigned char column,unsigned char * text,unsigned char txtlen,unsigned char charINV);
void Oled12832DisplayAsciiString8x16(unsigned char page,unsigned char column,unsigned char * text,unsigned char txtlen,unsigned char charINV);
void Oled12832ClearPage(unsigned char page,unsigned char len);
#endif