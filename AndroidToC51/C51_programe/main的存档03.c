#include "STC89C5X.h"
#include "intrins.h"
#include "panquanxing.h"
sbit T_STR = P0^5;       //锁存引脚，屏上标识位STR
sbit R1    = P0^4;       //数据引脚，屏上标识R1
sbit T_CLK = P0^6;       //时钟引脚，屏上标识位CLK
sbit OE    = P0^7;       //使能引脚，屏上标识EN/OE

unsigned int addrTest=0x00;
unsigned int countTest=0x00;

unsigned int startAddr=0;//用来保存初始地址的
unsigned int inputCount=0;//保存蓝牙输入的字符的个数
unsigned char wordCount=0;//保存在eeprom中的字符的个数

unsigned char MoveBitNum,MoveBitNumtemp,IncNum;//移动位数,临时移动位数，大于一个字节增加字节数
unsigned char HzNum;//汉字个数
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
	
	uchar count;//16行扫描数据，范围0-15
	uint i,j;
	uchar temp;

	//设置蓝牙接收波特率为9600Hz@22.1184MHz
	TMOD&=0x0f;
	TMOD|=0x20;//设置TMOD为0x20
	TH1=0xfa;
	TL1=0xfa;
	SCON=0x70;
	ET1=0;//禁止定时器1的中断
	TR1=1;//启动计时器一开始计时
	InitConfigure();
	wordCount=readLastDataOnSectorOne();
	wordCount+=1;
	wordCount>>=1;
	P2=wordCount;
	while(1){
			i++;
		//if(i==20)//更改流动速度,1T单片机和12T单片机速度大约5-8倍，注意更改参数
		if(i==40)
		{
			i=0;
			MoveBitNum++;
			if(MoveBitNum==16)//每次移动完一个汉字大小后循环
			{
					MoveBitNum=0;
					HzNum+=1;    //调用下一个汉字
					if(HzNum>=wordCount+4)//需要显示的汉字个数，包括前面的一屏空字符的个数，后面清屏的空字符不包含在内,这里是(汉字个数+4)
						 HzNum=0;   //完全显示完后循环调用
			}
		}

		delay(1);//控制扫描频率
		
		offset1=HzNum;
		offset1<<=5;
		offset2=HzNum;
		offset2<<=1;
		offset2+=1;
		offset2<<=4;
		wordCountBounder=wordCount;
		wordCountBounder<<=5;
		wordCountBounder+=0x0080;
		
		for(j=0;j<5;j++) //取每个汉字的前2个字节，4个汉字组成一行，数据传输完成后锁存输出即可，这里需要多取出一个汉字作为缓冲
		{                //原来j=4，现在j=5.
//			buff[j+j]=hztest[HzNum+HzNum][count];  //每次移动完一个汉字后，选择下一个汉字
//			buff[j+j+1]=hztest[HzNum+HzNum+1][count];
//			buff[j+j]=hztest[HzNum+HzNum+j+j][count];  //每次移动完一个汉字后，选择下一个汉字
//			buff[j+j+1]=hztest[HzNum+HzNum+1+j+j][count];
			EA=0;
			offset3=j;
			offset3<<=5;
			numCount1=0+offset1+offset3+count;
			numCount2=0+offset2+offset3+count;
			if(numCount1<0x0080||numCount1>=wordCountBounder){
				buff[j+j]=0x00;
			}else{
				//buff[j+j]=hztest[numCount1-0x0080];			
				buff[j+j]=EEPROM_ReadByte(0x2200+numCount1-0x0080);
			}
			
			if(numCount2<0x0080||numCount2>=wordCountBounder){
				buff[j+j+1]=0x00;
			}else{
				//buff[j+j+1]=hztest[numCount2-0x0080];
				buff[j+j+1]=EEPROM_ReadByte(0x2200+numCount2-0x0080);
			}
			EA=1;
		}
		if(MoveBitNum<8)                   //  判读移动距离是大于一个字节还是小于一个字节，因为一个字节左移右移最大只能8位
		{ 
			IncNum=0;
			MoveBitNumtemp=MoveBitNum; 
		}else{ 
			IncNum=1; 
			MoveBitNumtemp=MoveBitNum-8;//大于8就减去8得到的数值还是小于8
		}
		T_STR=0;       //锁存无效
		for(j=0;j<8;j++)          //按bit的方式移动缓冲区的内容，然后输出到595，即取出的数值每个字节左移一定的位数，
		{                                     //后面左移出的数据整合到前面的字节中，保持数据的连续性	     
			temp=(buff[j+IncNum]>>MoveBitNumtemp)|(buff[j+1+IncNum]<<(8-MoveBitNumtemp));//这句比较重要，需要自行拿出2个字节的数据模拟分析
			InputByte(temp);//输出到595
		}//8个字节传输完锁存输出
		OE = 1;
		P0=15-count;//用P0口的前4位控制16行，屏内部通过4-16译码器工作，循环扫描16行
		T_STR=1;      //锁存有效，此时一行的数据显示到屏上
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

//可能由于本人的技术问题，此函数只能定义在此，定义到另外的文件会无法写入EEPROM
void writeNewCheckCount(){	
	uint sectorOne=0x2000;
	EA=0;
	while(sectorOne<=0x21ff&&EEPROM_ReadByte(sectorOne)!=0xff){
		++sectorOne;
	}
		//这里可以通过而且sectorOne会每次自增1
	if(sectorOne>0x21ff){
		EEPROM_EraseSector(0x2000);
		EEPROM_WriteByte(0x2000,SBUF);//这里只能用SBUF,或者是指针,因为参数变量在不同的c文件，h文件不能传递太多次，容易造成无法接收到变量
	}else{
		EEPROM_WriteByte(sectorOne,SBUF);
	}
	EA=1;
}
void InputByte(uchar DataR1) //写一个字节
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

//void receiveData(){
//	if(RI){
//		startAddr=0x2200;
//	}
//}

void chuanKou() interrupt 4 using 1{
	if(!inputCount){
		startAddr=0x2200;//数据的
		writeNewCheckCount();//先把计数保存起来
		wordCount=SBUF;//最多96个字符，即可以装载96个英文字符，48个汉字
		inputCount=wordCount;//每个字符包含16个字节的字模数据
		inputCount<<=4;
		UART_EEPROM_EraseSector(0x2200);//擦除第三扇区
		UART_EEPROM_EraseSector(0x2400);//擦除第四扇区
		UART_EEPROM_EraseSector(0x2600);//擦除第五扇区
		//因为wordCount需要赋值给inputCount,所以只能最后转为Hzk字符数
		wordCount++;
		wordCount>>=1;//将wordCount转为对于的hzk字符个数
		RI=0;
		return;
	}else{
		--inputCount;
		UART_EEPROM_WriteByte(startAddr,SBUF);//写数据，每写一个字节地址自增1	
		startAddr+=1;
		//将数据尾部加5*2*16个0x00数据
		RI=0;
		return;
	}
}
