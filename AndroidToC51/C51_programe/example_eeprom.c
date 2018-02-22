/*------------------------------------------------------------------*/
/* --- STC MCU Limited ---------------------------------------------*/
/* --- STC89-90xx Series MCU ISP/IAP/EEPROM Demo -------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966----------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/

#include "reg51.h"
#include "intrins.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;

/*Declare SFR associated with the IAP */
sfr IAP_DATA    =   0xE2;           //Flash data register
sfr IAP_ADDRH   =   0xE3;           //Flash address HIGH
sfr IAP_ADDRL   =   0xE4;           //Flash address LOW
sfr IAP_CMD     =   0xE5;           //Flash command register
sfr IAP_TRIG    =   0xE6;           //Flash command trigger
sfr IAP_CONTR   =   0xE7;           //Flash control register

/*Define ISP/IAP/EEPROM command*/
#define CMD_IDLE    0               //Stand-By
#define CMD_READ    1               //Byte-Read
#define CMD_PROGRAM 2               //Byte-Program
#define CMD_ERASE   3               //Sector-Erase

/*Define ISP/IAP/EEPROM operation const for IAP_CONTR*/
//#define ENABLE_IAP 0x80           //if SYSCLK<40MHz
#define ENABLE_IAP   0x81           //if SYSCLK<20MHz
//#define ENABLE_IAP x82            //if SYSCLK<10MHz
//#define ENABLE_IAP 0x83           //if SYSCLK<5MHz

//Start address for STC89C58xx EEPROM
#define IAP_ADDRESS 0x08000

void Delay(BYTE n);
void IapIdle();
BYTE IapReadByte(WORD addr);
void IapProgramByte(WORD addr, BYTE dat);
void IapEraseSector(WORD addr);

void main()
{
    WORD i;

    P1 = 0xfe;                      //1111,1110 System Reset OK
    Delay(10);                      //Delay
    IapEraseSector(IAP_ADDRESS);    //Erase current sector
    for (i=0; i<512; i++)           //Check whether all sector data is FF
    {
        if (IapReadByte(IAP_ADDRESS+i) != 0xff)
            goto Error;             //If error, break
    }
    P1 = 0xfc;                      //1111,1100 Erase successful
    Delay(10);                      //Delay
    for (i=0; i<512; i++)           //Program 512 bytes data into data flash
    {
        IapProgramByte(IAP_ADDRESS+i, (BYTE)i);
    }
    P1 = 0xf8;                      //1111,1000 Program successful
    Delay(10);                      //Delay
    for (i=0; i<512; i++)           //Verify 512 bytes data
    {
        if (IapReadByte(IAP_ADDRESS+i) != (BYTE)i)
            goto Error;             //If error, break
    }
    P1 = 0xf0;                      //1111,0000 Verify successful
    while (1);
Error:
    P1 &= 0x7f;                     //0xxx,xxxx IAP operation fail
    while (1);
}

/*----------------------------
Software delay function
----------------------------*/
void Delay(BYTE n)
{
    WORD x;

    while (n--)
    {
        x = 0;
        while (++x);
    }
}

/*----------------------------
Disable ISP/IAP/EEPROM function
Make MCU in a safe state
----------------------------*/
void IapIdle()
{
    IAP_CONTR = 0;                  //Close IAP function
    IAP_CMD = 0;                    //Clear command to standby
    IAP_TRIG = 0;                   //Clear trigger register
    IAP_ADDRH = 0x80;               //Data ptr point to non-EEPROM area
    IAP_ADDRL = 0;                  //Clear IAP address to prevent misuse
}

/*----------------------------
Read one byte from ISP/IAP/EEPROM area
Input: addr (ISP/IAP/EEPROM address)
Output:Flash data
----------------------------*/
BYTE IapReadByte(WORD addr)
{
    BYTE dat;                       //Data buffer

    IAP_CONTR = ENABLE_IAP;         //Open IAP function, and set wait time
    IAP_CMD = CMD_READ;             //Set ISP/IAP/EEPROM READ command
    IAP_ADDRL = addr;               //Set ISP/IAP/EEPROM address low
    IAP_ADDRH = addr >> 8;          //Set ISP/IAP/EEPROM address high
    IAP_TRIG = 0x46;                //Send trigger command1 (0x46)
    IAP_TRIG = 0xb9;                //Send trigger command2 (0xb9)
    _nop_();                        //MCU will hold here until ISP/IAP/EEPROM operation complete
    dat = IAP_DATA;                 //Read ISP/IAP/EEPROM data
    IapIdle();                      //Close ISP/IAP/EEPROM function

    return dat;                     //Return Flash data
}

/*----------------------------
Program one byte to ISP/IAP/EEPROM area
Input: addr (ISP/IAP/EEPROM address)
       dat (ISP/IAP/EEPROM data)
Output:-
----------------------------*/
void IapProgramByte(WORD addr, BYTE dat)
{
    IAP_CONTR = ENABLE_IAP;         //Open IAP function, and set wait time
    IAP_CMD = CMD_PROGRAM;          //Set ISP/IAP/EEPROM PROGRAM command
    IAP_ADDRL = addr;               //Set ISP/IAP/EEPROM address low
    IAP_ADDRH = addr >> 8;          //Set ISP/IAP/EEPROM address high
    IAP_DATA = dat;                 //Write ISP/IAP/EEPROM data
    IAP_TRIG = 0x46;                //Send trigger command1 (0x46)
    IAP_TRIG = 0xb9;                //Send trigger command2 (0xb9)
    _nop_();                        //MCU will hold here until ISP/IAP/EEPROM operation complete
    IapIdle();
}

/*----------------------------
Erase one sector area
Input: addr (ISP/IAP/EEPROM address)
Output:-
----------------------------*/
void IapEraseSector(WORD addr)
{
    IAP_CONTR = ENABLE_IAP;         //Open IAP function, and set wait time
    IAP_CMD = CMD_ERASE;            //Set ISP/IAP/EEPROM ERASE command
    IAP_ADDRL = addr;               //Set ISP/IAP/EEPROM address low
    IAP_ADDRH = addr >> 8;          //Set ISP/IAP/EEPROM address high
    IAP_TRIG = 0x46;                //Send trigger command1 (0x46)
    IAP_TRIG = 0xb9;                //Send trigger command2 (0xb9)
    _nop_();                        //MCU will hold here until ISP/IAP/EEPROM operation complete
    IapIdle();
}

