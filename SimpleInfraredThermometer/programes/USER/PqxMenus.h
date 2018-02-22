#ifndef _PQXMENU_H_
#define _PQXMENU_H_

#ifndef PQXMENUS
#define PMExt extern
#else
#define PMExt 
#endif

#include "OLED12832.h"
extern unsigned char code home01[22];
extern unsigned char code home02[22];
extern unsigned char code home03[22];
extern unsigned char code go01[22];
extern unsigned char code go02[22];
extern unsigned char code go03[22];
extern unsigned char code go04[22];
extern unsigned char code set01[22];
extern unsigned char code set02[22];
extern unsigned char code set03[22];
extern unsigned char code set04[22];

extern unsigned char code noTitle[22];

extern unsigned char code Human[22];
extern unsigned char code Normal[22];
extern unsigned char code History[22];

extern unsigned char code EmTitle[22];
extern unsigned char code MiTitle[22];
extern unsigned char code MaTitle[22];

PMExt unsigned char* HOME[3];
PMExt unsigned char* GOGO[4];
PMExt unsigned char* SET[4];

PMExt unsigned char* HuItems[1];
PMExt unsigned char* NoItems[1];


PMExt unsigned char homeItem;
PMExt unsigned char goItem;
PMExt unsigned char setItem;
PMExt unsigned char pageItem;

PMExt unsigned char HuItem;
PMExt unsigned char NoItem;
PMExt unsigned char HiItem;

PMExt unsigned char MiItem;
PMExt unsigned char MaItem;
PMExt unsigned char EmItem;

PMExt float result;
PMExt float result02;
PMExt unsigned char isGeneral;

PMExt short readEepromAddr;
PMExt short writeEepromAddr;
PMExt short historyNum;
PMExt short historyCount;

PMExt unsigned char emissivity;
PMExt unsigned char lowerValue;
PMExt unsigned char upperValue;


void initPqxMenus(void);
void updatePage(unsigned char* page[],unsigned char len,unsigned char item);
void upAction(unsigned char *input);
void downAction(unsigned char *input);
#endif