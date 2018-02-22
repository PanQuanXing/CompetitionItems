#ifndef __PANQUANXING_H_
#define __PANQUANXING_H_

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

#define ISP_TRIG() ISP_TRIG=0x46,ISP_TRIG=0xB9

void disableEEPROM(void);
uchar EEPROM_ReadByte(uint);

void EEPROM_WriteByte(uint,uchar);

void EEPROM_EraseSector(uint);

#endif