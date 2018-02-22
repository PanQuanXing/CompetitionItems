#include "STC15F2K60S2.h"
#include "panquanxing.h"
#include "intrins.h"

void disableEEPROM(void){
	IAP_CONTR = 0;          //��ֹIAP/IAP����
	IAP_CMD   = 0;          //ȥ��IAP/IAP����
	IAP_TRIG  = 0;          //��ֹIAP/IAP�����󴥷�
	IAP_ADDRL = 0xff;       //��0��ַ���ֽ�
	IAP_ADDRH = 0xff;       //��0��ַ���ֽڣ�ָ���EEPROM������ֹ�����
}
uchar EEPROM_ReadByte(short addr){
	uchar dat;
	IAP_ADDRL=addr;//�͵�ַ��8λ
	IAP_ADDRH=addr>>8;//�͵�ַ��8λ
	IAP_CONTR=ENABLE_IAP;
	IAP_CMD=0x01;
	IAP_TRIG();
	_nop_();
	dat=IAP_DATA;//ȡ������
	disableEEPROM();
	return dat;
}

void EEPROM_WriteByte(short addr,uchar dat){
	IAP_ADDRL=addr;
	IAP_ADDRH=addr>>8;
	IAP_CONTR=ENABLE_IAP;
	IAP_CMD=0x02;//Flash/EEPROM�������ֽڱ������
	IAP_DATA=dat;//�����ݷ���IAP_DATA
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


