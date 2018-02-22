#include "includes.h"

void save2EEPROM() _task_ 5{
	int dataWrite;
	while(1){
		os_wait1(K_SIG);	
		if(writeEepromAddr<0x0400){
			dataWrite=TempC2Data(result);	
			EEPROM_WriteByte(writeEepromAddr,(dataWrite&0xff));
			writeEepromAddr+=1;
			dataWrite>>=8;
			EEPROM_WriteByte(writeEepromAddr,(dataWrite&0xff));
			writeEepromAddr+=1;
		}
		os_send_signal(0);
	}
}

void read2EEPROM() _task_ 6{
	short dataRead;
	while(1){
		os_wait1(K_SIG);	
		dataRead=EEPROM_ReadByte(readEepromAddr+1);
		dataRead<<=8;
		dataRead|=EEPROM_ReadByte(readEepromAddr);
		result=Data2TempC(dataRead);
		os_send_signal(0);
	}
}
