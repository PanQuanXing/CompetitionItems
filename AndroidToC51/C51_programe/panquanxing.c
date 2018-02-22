#include "panquanxing.h"
#include "STC89C5X.h"
#include "intrins.h"

void disableEEPROM(void){
	ISP_CONTR = 0;          //��ֹISP/IAP����
	ISP_CMD   = 0;          //ȥ��ISP/IAP����
	ISP_TRIG  = 0;          //��ֹISP/IAP�����󴥷�
	ISP_ADDRL = 0xff;       //��0��ַ���ֽ�
	ISP_ADDRH = 0xff;       //��0��ַ���ֽڣ�ָ���EEPROM������ֹ�����
}
uchar EEPROM_ReadByte(uint addr){
	uchar dat;
	ISP_ADDRL=addr;//�͵�ַ��8λ
	ISP_ADDRH=addr>>8;//�͵�ַ��8λ
	ISP_CONTR=0x81;
	ISP_CMD=0x01;
	ISP_TRIG();
	_nop_();
	dat=ISP_DATA;//ȡ������
	disableEEPROM();
	return dat;
}

void EEPROM_WriteByte(uint addr,uchar dat){
	ISP_ADDRL=addr;
	ISP_ADDRH=addr>>8;
	ISP_CONTR=0x81;
	ISP_CMD=0x02;//Flash/EEPROM�������ֽڱ������
	ISP_DATA=dat;//�����ݷ���ISP_DATA
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


