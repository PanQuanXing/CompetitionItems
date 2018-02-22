#include "includes.h"

void taskForChange() _task_ 7{
	
	while(1){
		os_wait1(K_SIG);
		ChangeEmissivity(0x5a,(float)(emissivity*0.1));
		os_send_signal(0);
	}
}