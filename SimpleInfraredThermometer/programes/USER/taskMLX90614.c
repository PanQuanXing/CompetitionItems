#include "includes.h"

void detectTemp() _task_ 4{
	unsigned char i;
	short tempValue;
	float temp;
	float maxResult;
	float minResult;
	while(1){
		os_wait1(K_SIG);
		if(isGeneral==1){
			result=Data2TempC(PqxMemRead(0x5a,0x07));
			result02=Data2TempC(PqxMemRead(0x5a,0x06));
			os_send_signal(0);
		}else if(isGeneral==0){
			temp=0;		
			result=0;
			maxResult=0;
			minResult=3.4E38;
			for(i=0;i<22;i++){
				temp=Data2TempC(PqxMemRead(0x5a,0x07));
				result+=temp;
				if(temp>maxResult){
					maxResult=temp;
				}
				if(temp<minResult){
					minResult=temp;
				}
			}
			result-=maxResult;
			result-=minResult;
			result*=0.05;
			os_send_signal(5);
		}else if(isGeneral==2){
			ChangeEmissivity(0x5a,(float)(emissivity*0.1));
			os_send_signal(0);
		}else if(isGeneral==3){
			tempValue=TempC2Data(lowerValue);	
			PqxEepromWrite(0x5a,0x21,(tempValue>>8),tempValue&0xff);
			os_send_signal(0);
		}else if(isGeneral==4){
			tempValue=TempC2Data(upperValue);
			PqxEepromWrite(0x5a,0x20,(tempValue>>8),tempValue&0xff);
			os_send_signal(0);
		}
	}
}

//void normalTemp() _task_ 5{
//	while(1){
//		os_wait1(K_SIG);	
//		result=0;
//		result02=0;
//		result=Data2TempC(PqxMemRead(0x5a,0x06));
//		result02=Data2TempC(PqxMemRead(0x5a,0x06));
//		os_send_signal(0);
//	}
//}
