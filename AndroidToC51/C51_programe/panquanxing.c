#include "panquanxing.h"
#include "STC89C5X.h"
#include "intrins.h"

void disableEEPROM(void){
	ISP_CONTR = 0;          //禁止ISP/IAP操作
	ISP_CMD   = 0;          //去除ISP/IAP命令
	ISP_TRIG  = 0;          //防止ISP/IAP命令误触发
	ISP_ADDRL = 0xff;       //清0地址高字节
	ISP_ADDRH = 0xff;       //清0地址低字节，指向非EEPROM区，防止误操作
}
uchar EEPROM_ReadByte(uint addr){
	uchar dat;
	ISP_ADDRL=addr;//送地址低8位
	ISP_ADDRH=addr>>8;//送地址高8位
	ISP_CONTR=0x81;
	ISP_CMD=0x01;
	ISP_TRIG();
	_nop_();
	dat=ISP_DATA;//取出数据
	disableEEPROM();
	return dat;
}

void EEPROM_WriteByte(uint addr,uchar dat){
	ISP_ADDRL=addr;
	ISP_ADDRH=addr>>8;
	ISP_CONTR=0x81;
	ISP_CMD=0x02;//Flash/EEPROM区进行字节编程命令
	ISP_DATA=dat;//将数据放入ISP_DATA
	ISP_TRIG();
	_nop_();
	disableEEPROM();
}

void EEPROM_EraseSector(uint addr){
	ISP_ADDRL=addr;
	ISP_ADDRH=addr>>8;
	ISP_CONTR=0x81;
	ISP_CMD=0x03;
	ISP_TRIG();
	_nop_();
	disableEEPROM();
}


