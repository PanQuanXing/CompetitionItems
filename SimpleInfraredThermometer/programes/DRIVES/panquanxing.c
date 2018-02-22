#include "STC15F2K60S2.h"
#include "panquanxing.h"
#include "intrins.h"

void disableEEPROM(void){
	IAP_CONTR = 0;          //禁止IAP/IAP操作
	IAP_CMD   = 0;          //去除IAP/IAP命令
	IAP_TRIG  = 0;          //防止IAP/IAP命令误触发
	IAP_ADDRL = 0xff;       //清0地址高字节
	IAP_ADDRH = 0xff;       //清0地址低字节，指向非EEPROM区，防止误操作
}
uchar EEPROM_ReadByte(short addr){
	uchar dat;
	IAP_ADDRL=addr;//送地址低8位
	IAP_ADDRH=addr>>8;//送地址高8位
	IAP_CONTR=ENABLE_IAP;
	IAP_CMD=0x01;
	IAP_TRIG();
	_nop_();
	dat=IAP_DATA;//取出数据
	disableEEPROM();
	return dat;
}

void EEPROM_WriteByte(short addr,uchar dat){
	IAP_ADDRL=addr;
	IAP_ADDRH=addr>>8;
	IAP_CONTR=ENABLE_IAP;
	IAP_CMD=0x02;//Flash/EEPROM区进行字节编程命令
	IAP_DATA=dat;//将数据放入IAP_DATA
	IAP_TRIG();
	_nop_();
	disableEEPROM();
}

void EEPROM_EraseSector(short addr){
	IAP_ADDRL=addr;
	IAP_ADDRH=addr>>8;
	IAP_CONTR=ENABLE_IAP;
	IAP_CMD=0x03;
	IAP_TRIG();
	_nop_();
	disableEEPROM();
}


