#include "PqxMLX90614.h"

void initMLX90614(){
	Pwm2Smbus();
}

static void Pwm2Smbus(){
	_SCL_OUTPUT();
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
	SCL=1;
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
	SCL=0;
	Delay_ms(4);
	SCL=1;
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
}

static void start_bit(){
	_SDA_OUTPUT();
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
	SDA=1;
	SCL=1;
	SmbusDelay10us();
	SDA=0;
	SmbusDelay10us();
	SCL=0;
	SmbusDelay10us();
}

static void stop_bit(){
	_SDA_OUTPUT();
	SDA=0;
	SCL=0;
	SmbusDelay10us();
	SDA=1;
	SmbusDelay10us();
	SCL=1;
	SmbusDelay20us();
}
static void SmbusAck(){
	_SDA_OUTPUT();
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
	SDA=0;
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
	SCL=1;
	SmbusDelay10us();
	SCL=0;
	SmbusDelay10us();
}

static void SmbusNack(){
	_SDA_OUTPUT();
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
	SDA=1;
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
	SCL=1;
	SmbusDelay10us();
	SCL=0;
	SmbusDelay10us();
}
//����8���ֽ����ݣ��Ұ����ȴ�Ӧ��λ�ĺ���
static bit send_byte(unsigned char ddata){
	unsigned char i=0;
	_SDA_OUTPUT();
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
	for(i=0;i<8;i++){
		if(ddata&0x80)
			SDA=1;
		else
			SDA=0;
		ddata<<=1;
		_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();
		SCL=1;
		SmbusDelay10us();
		SCL=0;
		SmbusDelay10us();
	}
	_SDA_INPUT();
	//��ʱ2us
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
	SCL=1;
	//��ʱ4us
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
	if(SDA)
	{
		//��ʱ4us
		_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();
		SCL=0;
		//��ʱ10us
		SmbusDelay10us();
		return false;
	}
	//��ʱ4us
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
	SCL=0;
	SmbusDelay10us();
	return true;
}


static unsigned char receive_byte(void){
	unsigned char i=8,ddata=0;
	_SDA_INPUT();
	//��ʱ2us
	_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();
	while(i--){
		//�����λ��ʼ����
		ddata<<=1;
		SCL=1;
		//��ʱ4us
		_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();
		if(SDA){
			ddata|=0x01;
		}else{
			ddata&=0xfe;
		}	
		//��ʱ2us
		_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();
		SCL=0;
		//��ʱ10us
		SmbusDelay10us();
	}
	return ddata;
}

short PqxMemRead(unsigned char slaveAddress,unsigned char cmd){
	unsigned char PEC=0;
	unsigned char MSG[5];
	int result=0;
	MSG[0]=(slaveAddress<<1);
	MSG[1]=cmd;
	MSG[2]=MSG[0]+1;
	
	start_bit();
	
	if(!send_byte(MSG[0])){
		stop_bit();
		return 0x0000;
	}		
		          
	if(!send_byte(MSG[1])){
		stop_bit();
		return 0x0000;
	}
	
	start_bit();
	
	if(!send_byte(MSG[2])){
		stop_bit();
		return 0x0000;	
	}
	
	MSG[3]=receive_byte();
	SmbusAck();
	
	MSG[4]=receive_byte();
	SmbusAck();
	
	PEC=receive_byte();
	if(PEC==CalculatePEC(MSG,5)){
		SmbusAck();
	}else{
		SmbusNack();
		stop_bit();
		return 0x0000;
	}
	stop_bit();
	
	result=MSG[4];
	result<<=8;
	result+=MSG[3];

	return result;
}

unsigned char CalculatePEC(unsigned char* msg,int len){ 
//MLX90614��У��ԭʼ����msgһ��Ϊ5λ��{SlaveAddress,Command,SlaveAddress+1,DataL,DataH};
     unsigned char i;        	//CRC8У�������             
     unsigned char ucCRC8=0;	//CRC8У�����ֽڳ�ʼ�� 
     //������CRCУ����            
     while(len--){  
         i=ucCRC8^(*msg);  
         msg++;
         ucCRC8=CRC8TAB[i];    
     }            
     return	ucCRC8;     
} 

float Data2TempK(int ddata){
	return ddata*0.02;
}

float Data2TempC(int ddata){//�˴�������short�ᶪʧ����
	float result=ddata*0.02;
	result-=273.15;
	return result;
}

int TempC2Data(float result){
	result+=273.15;
	result*=50;
	return (int)result;
}

//��MLX90614��EEPROM��д������
void PqxMemWrite(unsigned char slaveAddress,unsigned char cmd,unsigned char dataH,unsigned char dataL){
	unsigned char PEC;
	unsigned char MSG[4];
	MSG[0]=(slaveAddress<<1);
	MSG[1]=cmd;
//	MSG[2]=MSG[0]+1;
	MSG[2]=dataL;
	MSG[3]=dataH;
	PEC=CalculatePEC(MSG,4);
	
	start_bit();
	if(!send_byte(MSG[0])){
		stop_bit();
	}	
	if(!send_byte(MSG[1])){
		stop_bit();
	}
	if(!send_byte(MSG[2])){
		stop_bit();
	}	
	if(!send_byte(MSG[3])){
		stop_bit();
	}	
	if(!send_byte(PEC)){
		stop_bit();
	}	
	
	stop_bit();
	
	Delay_ms(10);
}

void PqxEepromWrite(unsigned char slaveAddress,unsigned char cmd,unsigned char dataH,unsigned char dataL){
	PqxMemWrite(slaveAddress,cmd,0x00,0x00);
	PqxMemWrite(slaveAddress,cmd,dataH,dataL);
}

bit EnterSleepMode(unsigned char slaveAddress){
	unsigned char MSG[2],PEC;
	MSG[0]=(slaveAddress<<1);
	MSG[1]=0xff;
//	PEC=CalculatePEC(MSG,2);//����PECУ����
	PEC=0xf3;
	start_bit();
	if(!send_byte(MSG[0])){
		stop_bit();
		return false;
	}	
	if(!send_byte(0xff)){
		stop_bit();
		return false;
	}
	if(!send_byte(PEC)){
		stop_bit();
		return false;
	}	
	stop_bit();
	return true;
}

//10����floatת16����
//Ĭ��byteArry�ĳ���lenΪsizeof(float)
//float����һ��4�ֽڣ�����λ����С�������С��
void Float2Emissivity(float e,unsigned char* dataH,unsigned char* dataL){
	int num;
	unsigned char i;
  unsigned char emissivityArry[4];
	i=0;
	num=(int)(e*65535+0.5);
	num&=0xffff;
	for(i=0;i<4;i++){
			emissivityArry[i]=num&0xf;
			num>>=4;
	}
	(*dataL)=(emissivityArry[1]<<4)+emissivityArry[0];   
	(*dataH)=(emissivityArry[3]<<4)+emissivityArry[2];		
}

void ChangeEmissivity(unsigned char slaveAddress,float emissivity){
	unsigned char EmL;
	unsigned char EmH;
	EA=0;//�����ٽ���
	Float2Emissivity(emissivity,&EmH,&EmL);
	PqxEepromWrite(slaveAddress,0x24,EmH,EmL);
	EnterSleepMode(slaveAddress);
	EA=1;//�˳��ٽ���
}
