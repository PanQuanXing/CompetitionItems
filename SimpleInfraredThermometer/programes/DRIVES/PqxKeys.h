#ifndef _PQXKEYS_H_
#define _PQXKEYS_H_
#include <rtx51tny.h>
#include "STC15F2K60S2.h"
#include "PqxDelay.h"

#define		KeyGpio		P1
#define 	KeyS			0x07

#define		KeyUpSetting()	P1M1 |= 0x01;P1M0 &=~0x01
#define		KeyDownSetting()	P1M1 |= 0x02;P1M0 &=~0x02
#define		KeyOkSetting()	P1M1 |= 0x04;P1M0 &=~0x04
void initKeys(void);
unsigned char KeysScan(void);
#endif
