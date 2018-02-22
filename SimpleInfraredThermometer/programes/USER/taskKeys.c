#include "includes.h"

void ScanKeyTask(void) _task_ 1{
	unsigned char keyValue;
	while(1){
		keyValue=KeysScan();
		if(keyValue!=0xff){
			keyValue=keyValue|(pageItem<<4);
			switch(keyValue){
				case 0x16://keyup
					if(homeItem>1)
						homeItem-=1;
					else
						homeItem=2;
					os_send_signal(0);
					break;
				case 0x15://keydown
					if(homeItem<2)
						homeItem+=1;
					else
						homeItem=1;
					os_send_signal(0);
					break;
				case 0x13://keyok
					pageItem+=homeItem;
					os_send_signal(0);
					break;
				
				case 0x26://keyup
					if(goItem>0)
						goItem-=1;
					else
						goItem=3;
					os_send_signal(0);
					break;
				case 0x25://keydown
					if(goItem<3)
						goItem+=1;
					else
						goItem=0;
					os_send_signal(0);
					break;
				case 0x23://keyok
						if(goItem==0){
							pageItem=1;
							os_send_signal(0);
						}else if(goItem==1){
							//擦除EEPROM
							EEPROM_EraseSector(MINADDR);
							EEPROM_EraseSector(SECONDADDR);
							writeEepromAddr=0x0000;//地址置0
							historyCount=0;
							pageItem=4;
							isGeneral=0;
							os_send_signal(4);
						}else if(goItem==2){
							pageItem=5;
							isGeneral=1;
							os_send_signal(4);
						}else if(goItem==3){//读取history
							readEepromAddr=0x0000;
							historyNum=0;
							pageItem=6;
							os_send_signal(6);
						}
					break;
				
				case 0x36://keyup
					if(setItem>0)
						setItem-=1;
					else
						setItem=3;				
					os_send_signal(0);
					break;
				case 0x35://keydown
					if(setItem<3)
						setItem+=1;
					else
						setItem=0;
					os_send_signal(0);
					break;
				case 0x33://keyok
					if(setItem==0){
							pageItem=1;
							os_send_signal(0);
						}else if(setItem==1){//改变lower
							pageItem=7;
							lowerValue=-40;
							os_send_signal(0);
						}else if(setItem==2){//改变upper
							upperValue=125;
							pageItem=8;
							os_send_signal(0);
						}else if(setItem==3){
							emissivity=10;
							pageItem=9;
							os_send_signal(0);
						}					
					break;
						
				case 0x46:
//					upAction(&HuItem);
					if(HuItem>0)
						HuItem-=1;
					else
						HuItem=1;
					os_send_signal(0);
					break;
				case 0x45:
					historyCount+=1;
					os_send_signal(4);
					break;
				case 0x43://ok
					if(HuItem==0){
						pageItem=2;
						os_send_signal(0);
					}
					break;
					
				case 0x56:
////					upAction(&NoItem);
//				NoItem=0;
					if(NoItem>0)
						NoItem-=1;
					else
						NoItem=1;
					os_send_signal(0);
					break;
				case 0x55:
					os_send_signal(4);
					break;
				case 0x53:
					if(NoItem==0){
						pageItem=2;
						os_send_signal(0);
					}
					break;
				
				case 0x66://up
					if(readEepromAddr<0x0400&&historyCount>0){
						historyCount-=1;
						readEepromAddr+=2;
						historyNum+=1;
						os_send_signal(6);
					}
					break;
				case 0x65://down
					if(readEepromAddr>0x0000){
						historyCount+=1;
						readEepromAddr-=2;
						historyNum-=1;
						os_send_signal(6);
					}
					break;
				case 0x63:
					pageItem=2;
					os_send_signal(0);
					break;
				//改变lower
				case 0x76:
					lowerValue+=1;
				os_send_signal(0);
					break;
				case 0x75:
					lowerValue-=1;
				os_send_signal(0);
					break;
				case 0x73:
					isGeneral=3;
					pageItem=3;
					os_send_signal(4);
					break;
				//改变upper
				case 0x86:
					upperValue+=1;
				os_send_signal(0);
					break;
				case 0x85:
					upperValue-=1;
				os_send_signal(0);
					break;
				case 0x83:
					isGeneral=4;
					pageItem=3;
					os_send_signal(4);
					break;
				
				case 0x96:
					if(emissivity<10){
						emissivity+=1;
					}
					os_send_signal(0);
					break;
				case 0x95:
					if(emissivity>0){
						emissivity-=1;
					}
					os_send_signal(0);
					break;
				case 0x93://ok
					isGeneral=2;
					pageItem=3;
					os_send_signal(4);
					break;
				
				default:
					break;
			}
			
		}
		os_switch_task();
	}
}
