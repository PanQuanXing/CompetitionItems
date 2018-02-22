

/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 -----------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.GXWMCU.com --------------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����        */
/*---------------------------------------------------------------------*/



/*************  ��������˵��  **************

                ����˵��

ͨ�����ڶ�STC�ڲ��Դ���EEPROM(FLASH)���ж�д���ԡ�

��FLASH������������д�롢�����Ĳ���������ָ����ַ��

Ĭ�ϲ�����:  115200,8,N,1. 
Ĭ����ʱ��:  11059200HZ.

������������: (������ĸ�����ִ�Сд)
    W 0x8000 1234567890  --> ��0x8000��ַд���ַ�1234567890.
    R 0x8000 10          --> ��0x8000��ַ����10���ֽ�����. 

ע�⣺Ϊ��ͨ�ã�����ʶ���ַ�Ƿ���Ч���û��Լ����ݾ�����ͺ���������

******************************************/

#include    "reg51.h"
#include    "intrins.h"

#define     MAIN_Fosc       11059200L   //������ʱ��

typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;

sfr TH2  = 0xD6;
sfr TL2  = 0xD7;
sfr IE2   = 0xAF;
sfr INT_CLKO = 0x8F;
sfr AUXR = 0x8E;
sfr AUXR1 = 0xA2;
sfr P_SW1 = 0xA2;
sfr P_SW2 = 0xBA;
sfr S2CON = 0x9A;
sfr S2BUF = 0x9B;

sfr ISP_DATA  = 0xC2;
sfr ISP_ADDRH = 0xC3;
sfr ISP_ADDRL = 0xC4;
sfr ISP_CMD   = 0xC5;
sfr ISP_TRIG  = 0xC6;
sfr ISP_CONTR = 0xC7;

sfr ADC_CONTR = 0xBC;   //��ADϵ��
sfr ADC_RES   = 0xBD;   //��ADϵ��
sfr ADC_RESL  = 0xBE;   //��ADϵ��
sfr P1ASF = 0x9D;   //ֻд��ģ������(AD��LVD)ѡ��

sfr P4   = 0xC0;
sfr P5   = 0xC8;
sfr P6   = 0xE8;
sfr P7   = 0xF8;
sfr P1M1 = 0x91;    //PxM1.n,PxM0.n     =00--->Standard,    01--->push-pull
sfr P1M0 = 0x92;    //                  =10--->pure input,  11--->open drain
sfr P0M1 = 0x93;
sfr P0M0 = 0x94;
sfr P2M1 = 0x95;
sfr P2M0 = 0x96;
sfr P3M1 = 0xB1;
sfr P3M0 = 0xB2;
sfr P4M1 = 0xB3;
sfr P4M0 = 0xB4;
sfr P5M1 = 0xC9;
sfr P5M0 = 0xCA;
sfr P6M1 = 0xCB;
sfr P6M0 = 0xCC;
sfr P7M1 = 0xE1;
sfr P7M0 = 0xE2;

sbit P00 = P0^0;
sbit P01 = P0^1;
sbit P02 = P0^2;
sbit P03 = P0^3;
sbit P04 = P0^4;
sbit P05 = P0^5;
sbit P06 = P0^6;
sbit P07 = P0^7;
sbit P10 = P1^0;
sbit P11 = P1^1;
sbit P12 = P1^2;
sbit P13 = P1^3;
sbit P14 = P1^4;
sbit P15 = P1^5;
sbit P16 = P1^6;
sbit P17 = P1^7;
sbit P20 = P2^0;
sbit P21 = P2^1;
sbit P22 = P2^2;
sbit P23 = P2^3;
sbit P24 = P2^4;
sbit P25 = P2^5;
sbit P26 = P2^6;
sbit P27 = P2^7;
sbit P30 = P3^0;
sbit P31 = P3^1;
sbit P32 = P3^2;
sbit P33 = P3^3;
sbit P34 = P3^4;
sbit P35 = P3^5;
sbit P36 = P3^6;
sbit P37 = P3^7;
sbit P40 = P4^0;
sbit P41 = P4^1;
sbit P42 = P4^2;
sbit P43 = P4^3;
sbit P44 = P4^4;
sbit P45 = P4^5;
sbit P46 = P4^6;
sbit P47 = P4^7;
sbit P50 = P5^0;
sbit P51 = P5^1;
sbit P52 = P5^2;
sbit P53 = P5^3;
sbit P54 = P5^4;
sbit P55 = P5^5;
sbit P56 = P5^6;
sbit P57 = P5^7;


#define     Baudrate1           115200L
#define     Tmp_Length          70      //��дEEPROM���峤��

#define     UART1_BUF_LENGTH    (Tmp_Length+9)  //���ڻ��峤��

u8  RX1_TimeOut;
u8  TX1_Cnt;    //���ͼ���
u8  RX1_Cnt;    //���ռ���
bit B_TX1_Busy; //����æ��־

u8  xdata   RX1_Buffer[UART1_BUF_LENGTH];   //���ջ���
u8  xdata   tmp[Tmp_Length];        //EEPROM��������


void    UART1_config(u8 brt);   // ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer1��������.
void    PrintString1(u8 *puts);
void    UART1_TxByte(u8 dat);
void    delay_ms(u8 ms);
u8      CheckData(u8 dat);
u16     GetAddress(void);
u8      GetDataLength(void);
void    EEPROM_SectorErase(u16 EE_address);
void    EEPROM_read_n(u16 EE_address,u8 *DataAddress,u8 length);
u8      EEPROM_write_n(u16 EE_address,u8 *DataAddress,u8 length);


/********************* ������ *************************/
void main(void)
{
    u8  i,j;
    u16 addr;
    u8  status;

    P0M1 = 0;   P0M0 = 0;   //����Ϊ׼˫���
    P1M1 = 0;   P1M0 = 0;   //����Ϊ׼˫���
    P2M1 = 0;   P2M0 = 0;   //����Ϊ׼˫���
    P3M1 = 0;   P3M0 = 0;   //����Ϊ׼˫���
    P4M1 = 0;   P4M0 = 0;   //����Ϊ׼˫���
    P5M1 = 0;   P5M0 = 0;   //����Ϊ׼˫���
    P6M1 = 0;   P6M0 = 0;   //����Ϊ׼˫���
    P7M1 = 0;   P7M0 = 0;   //����Ϊ׼˫���

    UART1_config(1);    // ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer1��������.
    EA = 1; //�������ж�

    PrintString1("STC15F2K60S2ϵ�е�Ƭ��EEPROM���Գ���, ����������������ʾ��:\r\n");    //SUART1����һ���ַ���
    PrintString1("W 0x8000 1234567890  --> ��0x8000��ַд���ַ�1234567890.\r\n");   //SUART1����һ���ַ���
    PrintString1("R 0x8000 10          --> ��0x8000��ַ����10���ֽ�����.\r\n"); //SUART1����һ���ַ���

    while(1)
    {
        delay_ms(1);
        if(RX1_TimeOut > 0)     //��ʱ����
        {
            if(--RX1_TimeOut == 0)
            {
        //      for(i=0; i<RX1_Cnt; i++)    UART1_TxByte(RX1_Buffer[i]);    //���յ�������ԭ������,���ڲ���

                status = 0xff;  //״̬��һ����0ֵ
                if((RX1_Cnt >= 10) && (RX1_Buffer[1] == ' ') && (RX1_Buffer[8] == ' ')) //�������Ϊ10���ֽ�
                {
                    for(i=0; i<8; i++)
                    {
                        if((RX1_Buffer[i] >= 'a') && (RX1_Buffer[i] <= 'z'))    RX1_Buffer[i] = RX1_Buffer[i] - 'a' + 'A';  //Сдת��д
                    }
                    addr = GetAddress();
                    if(addr < 63488)    //������0~123����
                    {
                        if(RX1_Buffer[0] == 'W')    //д��N���ֽ�
                        {
                            j = RX1_Cnt - 9;
                            if(j > Tmp_Length)  j = Tmp_Length; //Խ����
                            EEPROM_SectorErase(addr);           //��������
                            i = EEPROM_write_n(addr,&RX1_Buffer[9],j);      //дN���ֽ�
                            if(i == 0)
                            {
                                PrintString1("��д��");
                                if(j >= 100)    {UART1_TxByte(j/100+'0');   j = j % 100;}
                                if(j >= 10)     {UART1_TxByte(j/10+'0');    j = j % 10;}
                                UART1_TxByte(j%10+'0');
                                PrintString1("�ֽ�����!\r\n");
                            }
                            else    PrintString1("д�����!\r\n");
                            status = 0; //������ȷ
                        }

                        else if(RX1_Buffer[0] == 'R')   //PC���󷵻�N�ֽ�EEPROM����
                        {
                            j = GetDataLength();
                            if(j > Tmp_Length)  j = Tmp_Length; //Խ����
                            if(j > 0)
                            {
                                PrintString1("����");
                                UART1_TxByte(j/10+'0');
                                UART1_TxByte(j%10+'0');
                                PrintString1("���ֽ��������£�\r\n");
                                EEPROM_read_n(addr,tmp,j);
                                for(i=0; i<j; i++)  UART1_TxByte(tmp[i]);
                                UART1_TxByte(0x0d);
                                UART1_TxByte(0x0a);
                                status = 0; //������ȷ
                            }
                        }
                    }
                }
                if(status != 0) PrintString1("�������!\r\n");
                RX1_Cnt  = 0;   //����ֽ���
            }
        }
    }
}
//========================================================================


//========================================================================
// ����: void  delay_ms(unsigned char ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��, ����ֻ֧��1~255ms. �Զ���Ӧ��ʱ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void  delay_ms(u8 ms)
{
     unsigned int i;
     do{
          i = MAIN_Fosc / 14000;
          while(--i)    ;   //14T per loop
     }while(--ms);
}

//========================================================================
// ����: u8 CheckData(u8 dat)
// ����: ���ַ�"0~9,A~F��a~f"ת��ʮ������.
// ����: dat: Ҫ�����ַ�.
// ����: 0x00~0x0FΪ��ȷ. 0xFFΪ����.
// �汾: V1.0, 2012-10-22
//========================================================================
u8  CheckData(u8 dat)
{
    if((dat >= '0') && (dat <= '9'))        return (dat-'0');
    if((dat >= 'A') && (dat <= 'F'))        return (dat-'A'+10);
    return 0xff;
}

//========================================================================
// ����: u16    GetAddress(void)
// ����: ����������뷽ʽ�ĵ�ַ.
// ����: ��.
// ����: 16λEEPROM��ַ.
// �汾: V1.0, 2013-6-6
//========================================================================
u16 GetAddress(void)
{
    u16 address;
    u8  i,j;
    
    address = 0;
    if((RX1_Buffer[2] == '0') && (RX1_Buffer[3] == 'X'))
    {
        for(i=4; i<8; i++)
        {
            j = CheckData(RX1_Buffer[i]);
            if(j >= 0x10)   return 65535;   //error
            address = (address << 4) + j;
        }
        return (address);
    }
    return  65535;  //error
}

/**************** ��ȡҪ�������ݵ��ֽ��� ****************************/
u8  GetDataLength(void)
{
    u8  i;
    u8  length;
    
    length = 0;
    for(i=9; i<RX1_Cnt; i++)
    {
        if(CheckData(RX1_Buffer[i]) >= 10)  break;
        length = length * 10 + CheckData(RX1_Buffer[i]);
    }
    return (length);
}


//========================================================================
// ����: void PrintString1(u8 *puts)
// ����: ����1�����ַ���������
// ����: puts:  �ַ���ָ��.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void PrintString1(u8 *puts) //����һ���ַ���
{
    for (; *puts != 0;  puts++) UART1_TxByte(*puts);    //����ֹͣ��0����
}

//========================================================================
// ����: SetTimer2Baudraye(u16 dat)
// ����: ����Timer2�������ʷ�������
// ����: dat: Timer2����װֵ.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void    SetTimer2Baudraye(u16 dat)  // ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer1��������.
{
    AUXR &= ~(1<<4);    //Timer stop
    AUXR &= ~(1<<3);    //Timer2 set As Timer
    AUXR |=  (1<<2);    //Timer2 set as 1T mode
    TH2 = dat / 256;
    TL2 = dat % 256;
    IE2  &= ~(1<<2);    //��ֹ�ж�
    AUXR |=  (1<<4);    //Timer run enable
}

//========================================================================
// ����: void   UART1_config(u8 brt)
// ����: UART1��ʼ��������
// ����: brt: ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer1��������.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void    UART1_config(u8 brt)    // ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer1��������.
{
    /*********** ������ʹ�ö�ʱ��2 *****************/
    if(brt == 2)
    {
        AUXR |= 0x01;       //S1 BRT Use Timer2;
        SetTimer2Baudraye(65536UL - (MAIN_Fosc / 4) / Baudrate1);
    }

    /*********** ������ʹ�ö�ʱ��1 *****************/
    else
    {
        TR1 = 0;
        AUXR &= ~0x01;      //S1 BRT Use Timer1;
        AUXR |=  (1<<6);    //Timer1 set as 1T mode
        TMOD &= ~(1<<6);    //Timer1 set As Timer
        TMOD &= ~0x30;      //Timer1_16bitAutoReload;
        TH1 = (u8)((65536UL - (MAIN_Fosc / 4) / Baudrate1) / 256);
        TL1 = (u8)((65536UL - (MAIN_Fosc / 4) / Baudrate1) % 256);
        ET1 = 0;    //��ֹ�ж�
        INT_CLKO &= ~0x02;  //�����ʱ��
        TR1  = 1;
    }
    /*************************************************/

    SCON = (SCON & 0x3f) | 0x40;    //UART1ģʽ, 0x00: ͬ����λ���, 0x40: 8λ����,�ɱ䲨����, 0x80: 9λ����,�̶�������, 0xc0: 9λ����,�ɱ䲨����
//  PS  = 1;    //�����ȼ��ж�
    ES  = 1;    //�����ж�
    REN = 1;    //�������
    P_SW1 &= 0x3f;
    P_SW1 |= 0x80;      //UART1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 0x80: P1.6 P1.7 (����ʹ���ڲ�ʱ��)
//  PCON2 |=  (1<<4);   //�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE

    B_TX1_Busy = 0;
    TX1_Cnt = 0;
    RX1_Cnt = 0;
    RX1_TimeOut = 0;
}



//========================================================================
// ����: void   UART1_TxByte(u8 dat)
// ����: ����һ���ֽ�.
// ����: ��.
// ����: ��.
// �汾: V1.0, 2014-6-30
//========================================================================

void    UART1_TxByte(u8 dat)
{
    SBUF = dat;
    B_TX1_Busy = 1;
    while(B_TX1_Busy);
}


//========================================================================
// ����: void UART1_int (void) interrupt UART1_VECTOR
// ����: UART1�жϺ�����
// ����: nine.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void UART1_int (void) interrupt 4
{
    if(RI)
    {
        RI = 0;
        if(RX1_Cnt >= UART1_BUF_LENGTH) RX1_Cnt = 0;
        RX1_Buffer[RX1_Cnt] = SBUF;
        RX1_Cnt++;
        RX1_TimeOut = 5;
    }

    if(TI)
    {
        TI = 0;
        B_TX1_Busy = 0;
    }
}


/*
STC15F/L2KxxS2  �������䣬512�ֽ�/��������0x0000��ʼ��

     �ͺ�        ��С   ������  ��ʼ��ַ  ������ַ   MOVC��ƫ�Ƶ�ַ
STC15F/L2K08S2   53K   106����  0x0000  ~  0xD3FF        0x2000
STC15F/L2K16S2   45K    90����  0x0000  ~  0xB3FF        0x4000
STC15F/L2K24S2   37K    74����  0x0000  ~  0x93FF        0x6000
STC15F/L2K32S2   29K    58����  0x0000  ~  0x73FF        0x8000
STC15F/L2K40S2   21K    42����  0x0000  ~  0x53FF        0xA000
STC15F/L2K48S2   13K    26����  0x0000  ~  0x33FF        0xC000
STC15F/L2K56S2   5K     10����  0x0000  ~  0x13FF        0xE000
STC15F/L2K60S2   1K      2����  0x0000  ~  0x03FF        0xF000

STC15F/L2K61S2   ��EPROM, ����122������FLASH�����Բ�д ��ַ 0x0000~0xF3ff.
*/

#define     ISP_STANDBY()   ISP_CMD = 0     //ISP���������ֹ��
#define     ISP_READ()      ISP_CMD = 1     //ISP��������
#define     ISP_WRITE()     ISP_CMD = 2     //ISPд������
#define     ISP_ERASE()     ISP_CMD = 3     //ISP��������

#define     ISP_ENABLE()    ISP_CONTR = (ISP_EN + ISP_WAIT_FREQUENCY)
#define     ISP_DISABLE()   ISP_CONTR = 0; ISP_CMD = 0; ISP_TRIG = 0; ISP_ADDRH = 0xff; ISP_ADDRL = 0xff

#define ISP_EN          (1<<7)
#define ISP_SWBS        (1<<6)
#define ISP_SWRST       (1<<5)
#define ISP_CMD_FAIL    (1<<4)
#define ISP_WAIT_1MHZ   7
#define ISP_WAIT_2MHZ   6
#define ISP_WAIT_3MHZ   5
#define ISP_WAIT_6MHZ   4
#define ISP_WAIT_12MHZ  3
#define ISP_WAIT_20MHZ  2
#define ISP_WAIT_24MHZ  1
#define ISP_WAIT_30MHZ  0

#if (MAIN_Fosc >= 24000000L)
    #define     ISP_WAIT_FREQUENCY  ISP_WAIT_30MHZ
#elif (MAIN_Fosc >= 20000000L)
    #define     ISP_WAIT_FREQUENCY  ISP_WAIT_24MHZ
#elif (MAIN_Fosc >= 12000000L)
    #define     ISP_WAIT_FREQUENCY  ISP_WAIT_20MHZ
#elif (MAIN_Fosc >= 6000000L)
    #define     ISP_WAIT_FREQUENCY  ISP_WAIT_12MHZ
#elif (MAIN_Fosc >= 3000000L)
    #define     ISP_WAIT_FREQUENCY  ISP_WAIT_6MHZ
#elif (MAIN_Fosc >= 2000000L)
    #define     ISP_WAIT_FREQUENCY  ISP_WAIT_3MHZ
#elif (MAIN_Fosc >= 1000000L)
    #define     ISP_WAIT_FREQUENCY  ISP_WAIT_2MHZ
#else
    #define     ISP_WAIT_FREQUENCY  ISP_WAIT_1MHZ
#endif



//========================================================================
// ����: void DisableEEPROM(void)
// ����: ��ֹEEPROM.
// ����: none.
// ����: none.
// �汾: V1.0, 2014-6-30
//========================================================================
void DisableEEPROM(void)        //��ֹ����EEPROM
{
    ISP_CONTR = 0;              //��ֹISP/IAP����
    ISP_CMD   = 0;              //ȥ��ISP/IAP����
    ISP_TRIG  = 0;              //��ֹISP/IAP�����󴥷�
    ISP_ADDRH = 0xff;           //ָ���EEPROM������ֹ�����
    ISP_ADDRL = 0xff;           //ָ���EEPROM������ֹ�����
}

//========================================================================
// ����: void   EEPROM_Trig(void)
// ����: ����EEPROM����.
// ����: none.
// ����: none.
// �汾: V1.0, 2014-6-30
//========================================================================
void    EEPROM_Trig(void)
{
    F0 = EA;    //����ȫ���ж�
    EA = 0;     //��ֹ�ж�, ���ⴥ��������Ч
    ISP_TRIG = 0x5A;
    ISP_TRIG = 0xA5;                    //����5AH������A5H��ISP/IAP�����Ĵ�����ÿ�ζ���Ҫ���
                                        //����A5H��ISP/IAP������������������
                                        //CPU�ȴ�IAP��ɺ󣬲Ż����ִ�г���
    _nop_();
    _nop_();
    EA = F0;    //�ָ�ȫ���ж�
}

//========================================================================
// ����: void   EEPROM_SectorErase(u16 EE_address)
// ����: ����һ������.
// ����: EE_address:  Ҫ������EEPROM�������е�һ���ֽڵ�ַ.
// ����: none.
// �汾: V1.0, 2014-6-30
//========================================================================
void    EEPROM_SectorErase(u16 EE_address)
{
    ISP_ENABLE();                       //���õȴ�ʱ�䣬����ISP/IAP��������һ�ξ͹�
    ISP_ERASE();                        //�����, ������������������ı�ʱ����������������
                                        //ֻ������������û���ֽڲ�����512�ֽ�/������
                                        //����������һ���ֽڵ�ַ����������ַ��
    ISP_ADDRH = EE_address / 256;       //��������ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
    ISP_ADDRL = EE_address % 256;       //��������ַ���ֽ�
    EEPROM_Trig();                      //����EEPROM����
    DisableEEPROM();                    //��ֹEEPROM����
}

//========================================================================
// ����: void EEPROM_read_n(u16 EE_address,u8 *DataAddress,u8 lenth)
// ����: ��N���ֽں���.
// ����: EE_address:  Ҫ������EEPROM���׵�ַ.
//       DataAddress: Ҫ�������ݵ�ָ��.
//       length:      Ҫ�����ĳ���
// ����: 0: д����ȷ.  1: д�볤��Ϊ0����.  2: д�����ݴ���.
// �汾: V1.0, 2014-6-30
//========================================================================
void EEPROM_read_n(u16 EE_address,u8 *DataAddress,u8 length)
{
    ISP_ENABLE();                           //���õȴ�ʱ�䣬����ISP/IAP��������һ�ξ͹�
    ISP_READ();                             //���ֽڶ���������ı�ʱ����������������
    do
    {
        ISP_ADDRH = EE_address / 256;       //�͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
        ISP_ADDRL = EE_address % 256;       //�͵�ַ���ֽ�
        EEPROM_Trig();                      //����EEPROM����
        *DataAddress = ISP_DATA;            //��������������
        EE_address++;
        DataAddress++;
    }while(--length);

    DisableEEPROM();
}


//========================================================================
// ����: u8 EEPROM_write_n(u16 EE_address,u8 *DataAddress,u8 length)
// ����: дN���ֽں���.
// ����: EE_address:  Ҫд���EEPROM���׵�ַ.
//       DataAddress: Ҫд�����ݵ�ָ��.
//       length:      Ҫд��ĳ���
// ����: 0: д����ȷ.  1: д�볤��Ϊ0����.  2: д�����ݴ���.
// �汾: V1.0, 2014-6-30
//========================================================================
u8  EEPROM_write_n(u16 EE_address,u8 *DataAddress,u8 length)
{
    u8  i;
    u16 j;
    u8  *p;
    
    if(length == 0) return 1;   //����Ϊ0����

    ISP_ENABLE();                       //���õȴ�ʱ�䣬����ISP/IAP��������һ�ξ͹�
    i = length;
    j = EE_address;
    p = DataAddress;
    ISP_WRITE();                            //�����, ���ֽ�д����
    do
    {
        ISP_ADDRH = EE_address / 256;       //�͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
        ISP_ADDRL = EE_address % 256;       //�͵�ַ���ֽ�
        ISP_DATA  = *DataAddress;           //�����ݵ�ISP_DATA��ֻ�����ݸı�ʱ����������
        EEPROM_Trig();                      //����EEPROM����
        EE_address++;                       //��һ����ַ
        DataAddress++;                      //��һ������
    }while(--length);                       //ֱ������

    EE_address = j;
    length = i;
    DataAddress = p;
    i = 0;
    ISP_READ();                             //��N���ֽڲ��Ƚ�
    do
    {
        ISP_ADDRH = EE_address / 256;       //�͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
        ISP_ADDRL = EE_address % 256;       //�͵�ַ���ֽ�
        EEPROM_Trig();                      //����EEPROM����
        if(*DataAddress != ISP_DATA)        //������������Դ���ݱȽ�
        {
            i = 2;
            break;
        }
        EE_address++;
        DataAddress++;
    }while(--length);

    DisableEEPROM();
    return i;
}

