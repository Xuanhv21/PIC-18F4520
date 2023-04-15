/*
 * File:   main_MAX7219.c
 * Author: XUAN
 *
 * Created on January 21, 2022, 8:39 PM
 */


#include <xc.h>
#include "config.h"
#define _XTAL_FREQ 8000000
#define SCK PORTCbits.RC3       //Serial clock
#define SDI PORTCbits.RC4       //Serial Data in
#define SDO PORTCbits.RC5       //Serial Data out
#define SS PORTAbits.RA5        //Slave Select
#define SCK_dir TRISCbits.RC3
#define SDI_dir TRISCbits.RC4
#define SDO_dir TRISCbits.RC5
#define SS_dir TRISAbits.RA5


#define DECODE_MODE_REG     0x09
#define INTESITY_REG        0x0A
#define SCAN_LIMIT_REG      0x0B
#define SHUTDOWN_REG        0x0C
#define DISPLAY_TEST_REG    0x0F
//------------------------------------------------------------------------------
#define DISABLE_DECODE      0x00
#define DECODE_ALL          0xFF
#define BRIGHTNESS          0x0F
#define SCAN_LOW_NIBBLE     0x03
#define SCAN_ALL_DIGITS     0x07
#define SHUTDOWN_MODE       0x00
#define NORMAL_OPERATION    0x01
#define DISABLE_TEST_MODE   0x00
#define ENABLE_TEST_MODE    0x01

#define MAX7219_CS_LOAD PORTCbits.RC0
#define MAX7219_CS_LOAD_DIRECTION TRISCbits.RC0

void SPI_Init()
{
    SS_dir = 1;
    SCK_dir = 0;
    SDI_dir = 1 ;
    SDO_dir = 0;
    SS_dir = 0;
    SSPSTAT = 0x80;//0b10000000    
    SSPCON1 = 0x30;//0b00110000
}
void SPI_Write(char data)
{
    SSPBUF = data;
    while(SSPSTATbits.BF == 0);
}
char SPI_Read(char dummy)
{
    SSPBUF = dummy;
    while(SSPSTATbits.BF == 0);
    return SSPBUF;
}

void MAX7219_Write(char reg1,char data1)
{
    MAX7219_CS_LOAD  = 0;
    SPI_Write(reg1);
    SPI_Write(data1);
    MAX7219_CS_LOAD = 1;
}
void MAX7219_init()
{
  SPI_Init();

  MAX7219_Write(DECODE_MODE_REG,DECODE_ALL);
  MAX7219_Write(INTESITY_REG,BRIGHTNESS);
  MAX7219_Write(SCAN_LIMIT_REG,SCAN_ALL_DIGITS);
  MAX7219_Write(SHUTDOWN_REG,NORMAL_OPERATION);
  MAX7219_Write(DISPLAY_TEST_REG,ENABLE_TEST_MODE);
  MAX7219_Write(DISPLAY_TEST_REG,DISABLE_TEST_MODE);
}

void MAX7219Print(unsigned long Number,unsigned short NumberOfDigits)
{
    for(long i=1;Number>0||i-NumberOfDigits<=0;Number/=10,i++)
    {
        MAX7219_CS_LOAD = 0;
        SPI_Write(i);
        SPI_Write(Number%10);
        MAX7219_CS_LOAD = 1;
    }
}
void MAX7219DisplayOff()
{
    MAX7219_CS_LOAD = 0;
    SPI_Write(SHUTDOWN_REG);
    SPI_Write(DISABLE_DECODE);
    MAX7219_CS_LOAD = 1;
}

void Max7219DisplayOn()
{
  MAX7219_CS_LOAD=0;
  SPI_Write(SHUTDOWN_REG);
  SPI_Write(NORMAL_OPERATION);
  MAX7219_CS_LOAD=1;
}
void Max7219UpdateIntensity(char OnValue)
{
  if(OnValue>=0&&OnValue<=15)
  {
    MAX7219_CS_LOAD=0;
    SPI_Write(0x0a);
    SPI_Write(OnValue);
    MAX7219_CS_LOAD=1;
  }
}
unsigned long i;
void main(void) 
{
    MAX7219_CS_LOAD = 0;                       // Deselect DAC
    MAX7219_CS_LOAD_DIRECTION = 0;             // Set CS# pin as Output
    SPI_Init();                           // Initialize SPI module

    MAX7219_init();
    Max7219DisplayOn();
    Max7219UpdateIntensity(0x04);
    __delay_ms(1000);
    while(1)// loop forever
    {
    for(i=0;i<99999999;i++)//Count From 0 To 99999999 And Display on 8 Seven segemnts
    {
       MAX7219Print(i,7);
       __delay_ms(100);
    }
    }
    return;
}
