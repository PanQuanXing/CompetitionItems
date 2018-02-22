#include "includes.h"

void initTaskStart() _task_ 0{
	unsigned char ch[17];
	initKeys();
	OLED_Init();
	OLED_Clear();
	initMLX90614();
	initPqxMenus();
	os_create_task(1);
	os_create_task(4);
	os_create_task(5);
	os_create_task(6);
	os_create_task(7);
	while(1){
		if(pageItem==1){
			updatePage(HOME,3,homeItem);
			Oled12832ClearPage(4,128);			
		}else if(pageItem==2){
			updatePage(GOGO,4,goItem);
		}else if(pageItem==3){
			updatePage(SET,4,setItem);
		}else if(pageItem==4){
			updatePage(HuItems,1,HuItem);
			Oled12832ClearPage(2,128);
			Oled12832ClearPage(3,128);
			Oled12832ClearPage(4,128);
			sprintf(ch,"[%f^C]", result);
			Oled12832DisplayAsciiString8x16(2,2,(unsigned char*)ch,(unsigned char)strlen((const char*)ch),0);
		}else if(pageItem==5){
			updatePage(HuItems,1,NoItem);
			Oled12832ClearPage(2,128);
			Oled12832ClearPage(3,128);
			Oled12832ClearPage(4,128);
			Oled12832DisplayAsciiString6x8(2,1,noTitle,21,0);
			sprintf(ch,"%0.2f^C", result);
			Oled12832DisplayAsciiString8x16(3,0,(unsigned char*)ch,(unsigned char)strlen((const char*)ch),0);
			sprintf(ch,"%0.2f^C", result02);
			Oled12832DisplayAsciiString8x16(3,60,(unsigned char*)ch,(unsigned char)strlen((const char*)ch),0);
		}else if(pageItem==6){
			Oled12832DisplayAsciiString6x8(1,2,History,21,0);
			Oled12832ClearPage(2,128);
			Oled12832ClearPage(3,128);
			Oled12832ClearPage(4,128);
			sprintf(ch,"%d--(%0.2f^C)", historyNum,result);
			Oled12832DisplayAsciiString8x16(3,2,(unsigned char*)ch,(unsigned char)strlen((const char*)ch),0);
		}else if(pageItem==7){//改变lower
			Oled12832DisplayAsciiString6x8(1,2,MiTitle,21,0);
			Oled12832ClearPage(2,128);
			Oled12832ClearPage(3,128);
			Oled12832ClearPage(4,128);
			sprintf(ch,"----( %d^C )---",lowerValue);
			Oled12832DisplayAsciiString8x16(3,2,(unsigned char*)ch,(unsigned char)strlen((const char*)ch),0);
		}else if(pageItem==8){//改变upper
			Oled12832DisplayAsciiString6x8(1,2,MaTitle,21,0);
			Oled12832ClearPage(2,128);
			Oled12832ClearPage(3,128);
			Oled12832ClearPage(4,128);
			sprintf(ch,"----( %d^C )---",upperValue);
			Oled12832DisplayAsciiString8x16(3,2,(unsigned char*)ch,(unsigned char)strlen((const char*)ch),0);
		}else if(pageItem==9){
			Oled12832DisplayAsciiString6x8(1,2,EmTitle,21,0);
			Oled12832ClearPage(2,128);
			Oled12832ClearPage(3,128);
			Oled12832ClearPage(4,128);
			sprintf(ch,"----( %0.1f )---",(emissivity*0.1));
			Oled12832DisplayAsciiString8x16(3,2,(unsigned char*)ch,(unsigned char)strlen((const char*)ch),0);
		}
		os_wait1(K_SIG);
	}
}

