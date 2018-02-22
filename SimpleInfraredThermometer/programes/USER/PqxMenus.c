#define PQXMENUS
#include "PqxMenus.h"
extern unsigned char code home01[22]	="********START********";
extern unsigned char code home02[22]	="-->GO                ";
extern unsigned char code home03[22]	="-->SETTING           ";
extern unsigned char code go01[22]		="*********GO**********";
extern unsigned char code go02[22]		="->Body Temperature   ";
extern unsigned char code go03[22]		="->Normal Temperature ";
extern unsigned char code go04[22]		="->History            ";
extern unsigned char code set01[22] 	="*******SETTING*******";
extern unsigned char code set02[22] 	="->Lower Limit        ";
extern unsigned char code set03[22] 	="->Upper Limit        ";
extern unsigned char code set04[22] 	="->Emissivity         ";

extern unsigned char code noTitle[22]	="Tobj:    Tamb:      ";

extern unsigned char code Human[22] 	=">>Shell Temperature<<";
extern unsigned char code Normal[22]	=">>>>>>Generally<<<<<<";
extern unsigned char code History[22]	="-------History-------";

extern unsigned char code EmTitle[22]	="--Change Emissivity--";
extern unsigned char code MiTitle[22]	="--ChangeLowerLimit---";
extern unsigned char code MaTitle[22]	="--ChangeUpperLimit---";

void initPqxMenus(void){

homeItem=0;
goItem=0;
setItem=0;
pageItem=1;

HuItem=1;
NoItem=1;
HiItem=1;

MiItem=1;
MaItem=1;
EmItem=1;
	
	
	
	HOME[0]=home01;
	HOME[1]=home02;
	HOME[2]=home03;

	GOGO[0]=go01;
	GOGO[1]=go02;
	GOGO[2]=go03;
	GOGO[3]=go04;

  SET[0]=set01;
	SET[1]=set02;
	SET[2]=set03;
	SET[3]=set04;
	
	HuItems[0]=Human;
	NoItems[0]=Normal;
}

void updatePage(unsigned char* page[],unsigned char len,unsigned char item){
	unsigned char i;
	for(i=0;i<len;i++){
		if(i==item)
			Oled12832DisplayAsciiString6x8(i+1,2,page[i],21,1);
		else
			Oled12832DisplayAsciiString6x8(i+1,2,page[i],21,0);
	}
	
}

void upAction(unsigned char *input){
	if((*input)>0)
		(*input)-=1;
	else
		(*input)=1;
}
void downAction(unsigned char *input){
	if((*input)<1)
		(*input)+=1;
	else
		(*input)=0;
}
