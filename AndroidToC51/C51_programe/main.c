#include "STC89C5X.h"
#include "intrins.h"
#include "panquanxing.h"
sbit T_STR = P0^5;       //�������ţ����ϱ�ʶλSTR
sbit R1    = P0^4;       //�������ţ����ϱ�ʶR1
sbit T_CLK = P0^6;       //ʱ�����ţ����ϱ�ʶλCLK
sbit OE    = P0^7;       //ʹ�����ţ����ϱ�ʶEN/OE
unsigned char isEven;
unsigned int startAddr=0;//���������ʼ��ַ��
unsigned int inputCount=0;//��������������ַ��ĸ���
unsigned char wordCount=0;//������eeprom�е��ַ��ĸ���

unsigned char MoveBitNum,MoveBitNumtemp,IncNum;//�ƶ�λ��,��ʱ�ƶ�λ��������һ���ֽ������ֽ���
unsigned char HzNum;//���ָ���
unsigned char buff[10]={0};

unsigned int offset1=0,offset2=0,offset3=0;
unsigned int numCount1=0,numCount2=0;
unsigned int wordCountBounder;

void InitConfigure(void);
void writeNewCheckCount();
void InputByte(uchar);
void receiveData();
uchar myReadLastDataOnSector();
void EEPROM_Init_Zero(uint);
void myDelay(uchar);
void main(void){
	uchar count;//16��ɨ�����ݣ���Χ0-15
	uint i,j;
	uchar temp;

	//�����������ղ�����Ϊ9600Hz@22.1184MHz
	TMOD&=0x0f;
	TMOD|=0x20;//����TMODΪ0x20
	TH1=0xfa;
	TL1=0xfa;
	SCON=0x70;
	ET1=0;//��ֹ��ʱ��1���ж�
	TR1=1;//������ʱ��һ��ʼ��ʱ
	
	wordCount=myReadLastDataOnSector();
	wordCount+=1;
	wordCount>>=1;
	P2=wordCount;
	while(1){
			i++;
		if(inputCount==1){
				inputCount=0;
		}
		receiveData();
		//if(i==20)//���������ٶ�,1T��Ƭ����12T��Ƭ���ٶȴ�Լ5-8����ע����Ĳ���
		if(i==60)
		{
			i=0;
			MoveBitNum++;
			if(MoveBitNum==16)//ÿ���ƶ���һ�����ִ�С��ѭ��
			{
					MoveBitNum=0;
					HzNum+=1;    //������һ������
					if(HzNum>=wordCount+4)//��Ҫ��ʾ�ĺ��ָ���������ǰ���һ�����ַ��ĸ��������������Ŀ��ַ�����������,������(���ָ���+4)
						 HzNum=0;   //��ȫ��ʾ���ѭ������
			}
		}

		myDelay(1);//����ɨ��Ƶ��
		
		offset1=HzNum;
		offset1<<=5;
		offset2=HzNum;
		offset2<<=1;
		offset2+=1;
		offset2<<=4;
		wordCountBounder=wordCount;
		wordCountBounder<<=5;
		wordCountBounder+=0x0080;
		
		for(j=0;j<5;j++) //ȡÿ�����ֵ�ǰ2���ֽڣ�4���������һ�У����ݴ�����ɺ�����������ɣ�������Ҫ��ȡ��һ��������Ϊ����
		{                //ԭ��j=4������j=5.

			offset3=j;
			offset3<<=5;
			numCount1=0+offset1+offset3+count;
			numCount2=0+offset2+offset3+count;
			if(numCount1<0x0080||numCount1>=wordCountBounder){
				buff[j+j]=0x00;
			}else{		
				buff[j+j]=EEPROM_ReadByte(0x2200+numCount1-0x0080);
			}
			
			if(numCount2<0x0080||numCount2>=wordCountBounder){
				buff[j+j+1]=0x00;
			}else{
				buff[j+j+1]=EEPROM_ReadByte(0x2200+numCount2-0x0080);
			}
		}
		if(MoveBitNum<8)                   //  �ж��ƶ������Ǵ���һ���ֽڻ���С��һ���ֽڣ���Ϊһ���ֽ������������ֻ��8λ
		{ 
			IncNum=0;
			MoveBitNumtemp=MoveBitNum; 
		}else{ 
			IncNum=1; 
			MoveBitNumtemp=MoveBitNum-8;//����8�ͼ�ȥ8�õ�����ֵ����С��8
		}
		T_STR=0;       //������Ч
		for(j=0;j<8;j++)          //��bit�ķ�ʽ�ƶ������������ݣ�Ȼ�������595����ȡ������ֵÿ���ֽ�����һ����λ����
		{                                     //�������Ƴ����������ϵ�ǰ����ֽ��У��������ݵ�������	     
			temp=(buff[j+IncNum]>>MoveBitNumtemp)|(buff[j+1+IncNum]<<(8-MoveBitNumtemp));//���Ƚ���Ҫ����Ҫ�����ó�2���ֽڵ�����ģ�����
			InputByte(temp);//�����595
		}//8���ֽڴ������������
		OE = 1;
		P0=15-count;//��P0�ڵ�ǰ4λ����16�У����ڲ�ͨ��4-16������������ѭ��ɨ��16��
		T_STR=1;      //������Ч����ʱһ�е�������ʾ������
		OE = 0;
		count++;
		if(count==16)
			count=0;
	}
}

//�������ڱ��˵ļ������⣬�˺���ֻ�ܶ����ڴˣ����嵽������ļ����޷�д��EEPROM
void writeNewCheckCount(){	
	uint sectorOne=0x2000;
	while(sectorOne<=0x21ff&&EEPROM_ReadByte(sectorOne)!=0xff){
		++sectorOne;
	}
		//�������ͨ������sectorOne��ÿ������1
	if(sectorOne>0x21ff){
		EEPROM_EraseSector(0x2000);
		EEPROM_WriteByte(0x2000,SBUF);//����ֻ����SBUF,������ָ��,��Ϊ���������ڲ�ͬ��c�ļ���h�ļ����ܴ���̫��Σ���������޷����յ�����
	}else{
		EEPROM_WriteByte(sectorOne,SBUF);
	}
}
void InputByte(uchar DataR1) //дһ���ֽ�
{ 
	uchar i;
	for(i=8; i>0; i--)
	{
		R1 = !(DataR1&0x01);                   
		T_CLK = 0;
		T_CLK = 1;
		DataR1 = DataR1 >> 1; 
	} 
}

uchar myReadLastDataOnSector(){//�������ж���ʹ�ô˺���
	uchar checkCount;
	uint sectorOne=0x2000;
	while(sectorOne<=0x21ff&&EEPROM_ReadByte(sectorOne)!=0xff){
		++sectorOne;
	}
	if(sectorOne>0x2000){
		checkCount=EEPROM_ReadByte(sectorOne-1);
	}else{
		checkCount=0;//null value for write!
	}
	return checkCount;
}

void receiveData(){
	if(RI&&inputCount==0){
		startAddr=0x2200;//���ݵ�
		writeNewCheckCount();//�ȰѼ�����������
		wordCount=SBUF;//���96���ַ���������װ��96��Ӣ���ַ���48������
		inputCount=wordCount;//ÿ���ַ�����16���ֽڵ���ģ����
		if(wordCount%2){
			isEven=1;
		}else{
			isEven=0;
		}
		inputCount<<=4;
		EEPROM_EraseSector(0x2200);//������������
		EEPROM_EraseSector(0x2400);//������������
		EEPROM_EraseSector(0x2600);//������������
		//��ΪwordCount��Ҫ��ֵ��inputCount,����ֻ�����תΪHzk�ַ���
		wordCount++;
		wordCount>>=1;//��wordCountתΪ���ڵ�hzk�ַ�����
		while(inputCount){
			RI=0;
			inputCount-=1;
			while(RI==0);
			EEPROM_WriteByte(startAddr,SBUF);//д���ݣ�ÿдһ���ֽڵ�ַ����1
			startAddr+=1;
		}
	}
	if(isEven){
		EEPROM_Init_Zero(startAddr);
		isEven=0;
	}
	RI=0;
}
void EEPROM_Init_Zero(uint startAddr){
	uchar num=16;
	ISP_CONTR=0x81;
	ISP_CMD=0x02;
	while(num){
		num-=1;
		ISP_ADDRL=startAddr;
		ISP_ADDRH=startAddr>>8;
		ISP_DATA=0x00;
		ISP_TRIG();
		_nop_();
		startAddr+=1;
	}
	disableEEPROM();
}
void myDelay(uchar t)
{
 while(t){
	 t-=1;
 }
}