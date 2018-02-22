#include "STC89C5X.h"
#include "intrins.h"
#include "panquanxing.h"
sbit T_STR = P0^5;       //�������ţ����ϱ�ʶλSTR
sbit R1    = P0^4;       //�������ţ����ϱ�ʶR1
sbit T_CLK = P0^6;       //ʱ�����ţ����ϱ�ʶλCLK
sbit OE    = P0^7;       //ʹ�����ţ����ϱ�ʶEN/OE

unsigned int addrTest=0x00;
unsigned int countTest=0x00;

unsigned int startAddr=0;//���������ʼ��ַ��
unsigned int inputCount=0;//��������������ַ��ĸ���
unsigned char wordCount=0;//������eeprom�е��ַ��ĸ���

unsigned char MoveBitNum,MoveBitNumtemp,IncNum;//�ƶ�λ��,��ʱ�ƶ�λ��������һ���ֽ������ֽ���
unsigned char HzNum;//���ָ���
unsigned char buff[10]={0};

unsigned int offset1=0,offset2=0,offset3=0;
unsigned int numCount1=0,numCount2=0;
unsigned int wordCountBounder;

unsigned char xdata hztest[]={
0x08,0x08,0xF8,0x08,0xDF,0x48,0x48,0xD8,0x0C,0xEB,0x28,0xA8,0xA8,0xA8,0x2A,0x24,
0x01,0x22,0x7F,0x00,0x1F,0x10,0x10,0x1F,0x20,0x7F,0x20,0x2F,0x28,0x2F,0x20,0x30,

0x08,0x08,0xF8,0x08,0xDF,0x48,0x48,0xD8,0x0C,0xEB,0x28,0xA8,0xA8,0xA8,0x2A,0x24,
0x01,0x22,0x7F,0x00,0x1F,0x10,0x10,0x1F,0x20,0x7F,0x20,0x2F,0x28,0x2F,0x20,0x30,
	
0x08,0x10,0x7E,0x42,0x42,0x7E,0x42,0x42,0x7E,0x02,0x12,0x22,0x5A,0x46,0x82,0x40,
0x00,0x3E,0x22,0x22,0x22,0x3E,0x22,0x22,0x22,0x3E,0x22,0x22,0x21,0x21,0x28,0x10,

0x08,0x10,0x7E,0x42,0x42,0x7E,0x42,0x42,0x7E,0x02,0x12,0x22,0x5A,0x46,0x82,0x40,
0x00,0x3E,0x22,0x22,0x22,0x3E,0x22,0x22,0x22,0x3E,0x22,0x22,0x21,0x21,0x28,0x10,

0x08,0x10,0x7E,0x42,0x42,0x7E,0x42,0x42,0x7E,0x02,0x12,0x22,0x5A,0x46,0x82,0x40,
0x00,0x3E,0x22,0x22,0x22,0x3E,0x22,0x22,0x22,0x3E,0x22,0x22,0x21,0x21,0x28,0x10,

0x08,0x10,0x7E,0x42,0x42,0x7E,0x42,0x42,0x7E,0x02,0x12,0x22,0x5A,0x46,0x82,0x40,
0x00,0x3E,0x22,0x22,0x22,0x3E,0x22,0x22,0x22,0x3E,0x22,0x22,0x21,0x21,0x28,0x10,

0x08,0x10,0x7E,0x42,0x42,0x7E,0x42,0x42,0x7E,0x02,0x12,0x22,0x5A,0x46,0x82,0x40,
0x00,0x3E,0x22,0x22,0x22,0x3E,0x22,0x22,0x22,0x3E,0x22,0x22,0x21,0x21,0x28,0x10
};
void EEPROM_Init_Zero(uint);
void InitConfigure(void);
void writeNewCheckCount();
void InputByte(uchar);
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
	InitConfigure();
	wordCount=readLastDataOnSectorOne();
	wordCount+=1;
	wordCount>>=1;
	P2=wordCount;
	while(1){
			i++;
		//if(i==20)//���������ٶ�,1T��Ƭ����12T��Ƭ���ٶȴ�Լ5-8����ע����Ĳ���
		if(i==40)
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

		delay(1);//����ɨ��Ƶ��
		
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
//			buff[j+j]=hztest[HzNum+HzNum][count];  //ÿ���ƶ���һ�����ֺ�ѡ����һ������
//			buff[j+j+1]=hztest[HzNum+HzNum+1][count];
//			buff[j+j]=hztest[HzNum+HzNum+j+j][count];  //ÿ���ƶ���һ�����ֺ�ѡ����һ������
//			buff[j+j+1]=hztest[HzNum+HzNum+1+j+j][count];

			offset3=j;
			offset3<<=5;
			numCount1=0+offset1+offset3+count;
			numCount2=0+offset2+offset3+count;
			if(numCount1<0x0080||numCount1>=wordCountBounder){
				buff[j+j]=0x00;
			}else{
				//buff[j+j]=hztest[numCount1-0x0080];			
				//buff[j+j]=EEPROM_ReadByte(0x2200+numCount1-0x0080);
			}
			
			if(numCount2<0x0080||numCount2>=wordCountBounder){
				buff[j+j+1]=0x00;
			}else{
				//buff[j+j+1]=hztest[numCount2-0x0080];
				//buff[j+j+1]=EEPROM_ReadByte(0x2200+numCount2-0x0080);
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
void InitConfigure(void){
	ES=1;
	EA=1;
}

//�������ڱ��˵ļ������⣬�˺���ֻ�ܶ����ڴˣ����嵽������ļ����޷�д��EEPROM
void writeNewCheckCount(){	
	uint sectorOne=0x2000;
	EA=0;
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

void receiveData(){
	if(RI){
		startAddr=0x2200;
	}
}

void chuanKou() interrupt 4 using 1{
	if(!inputCount){
		startAddr=0x2200;//���ݵ�
		writeNewCheckCount();//�ȰѼ�����������
		wordCount=SBUF;//���96���ַ���������װ��96��Ӣ���ַ���48������
		inputCount=wordCount;//ÿ���ַ�����16���ֽڵ���ģ����
		inputCount<<=4;
		UART_EEPROM_EraseSector(0x2200);//������������
		UART_EEPROM_EraseSector(0x2400);//������������
		UART_EEPROM_EraseSector(0x2600);//������������
		//��ΪwordCount��Ҫ��ֵ��inputCount,����ֻ�����תΪHzk�ַ���
		wordCount++;
		wordCount>>=1;//��wordCountתΪ���ڵ�hzk�ַ�����
		RI=0;
		return;
	}else{
		--inputCount;
		UART_EEPROM_WriteByte(startAddr,SBUF);//д���ݣ�ÿдһ���ֽڵ�ַ����1	
		startAddr+=1;
		//������β����5*2*16��0x00����
		RI=0;
		return;
	}
}
