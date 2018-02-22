#ifndef __PANQUANXING_H_
#define __PANQUANXING_H_

#ifndef uchar
#define uchar unsigned char
#endif


#define ENABLE_IAP 0x83
#define IAP_TRIG() IAP_TRIG=0x5a,IAP_TRIG=0xa5
#define MINADDR	0x0000
#define SECONDADDR 0x200
#define MAXADDR 0x03ff
void disableEEPROM(void);
uchar EEPROM_ReadByte(short);

void EEPROM_WriteByte(short,uchar);

void EEPROM_EraseSector(short);

#endif