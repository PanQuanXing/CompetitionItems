#include "reg52.h"
#include "intrins.h"
void delay(unsigned int);
void InitConfigure(void);
unsigned char datac;
void main(void){
	TMOD=0x20;
	TH1=0xfd;
	TL1=0xfd;
	SCON=0x70;
	TR1=1;
	InitConfigure();
	while(1){
		P0=datac;
		delay(1000);
	}
}
void delay(unsigned int msec){
	unsigned int i,j;
	for(i=msec;i>0;--i)
		for(j=112;j>0;--j);
}
void InitConfigure(void){
	ES=1;
	EA=1;
}
void chuanKou(void) interrupt 4 using 1{
	datac=SBUF;
	RI=0;
}