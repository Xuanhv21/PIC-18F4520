/*
 * File:   main.c
 * Author: XUAN
 *
 * Created on December 26, 2021, 10:05 AM
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

#define WREN 0x06     //0b00000110          //Write Enable
#define WRDI 0x04     //0b00000100          //Write Disable
#define READ 0x03     //0b00000011
#define WRITE 0x02    //0b00000010
#define RDSR 0x05     //0b00000101          //Read Status Register
char data[16] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
int address[16] = {0x0103,0x0B31,0x1032,0x0C23,0x6004,0xC0BF,0x10C6,0x0317,0x0AF8,0x0A19,0x102A,0x0CCB,0x1388,0x612D,0x501E,0x42AF};
/* Addresses are 16 bit(hence int data type) wide <ie 4 nibbles>(According to Datasheet info).
 *  NB: Chosen at Random */
void EEPROM_Write(int address,char data);
char EEPROM_Read(int address);
char EEPROM_Status();
char EEPROM_Busy();
char EEPROM_wrEnable(); 

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

void main(void) 
{
    SPI_Init();
    TRISD = 0;
    PORTD = data[0];
    for(int i=0;i<16;i++)
    {
        EEPROM_Write(address[i],data[i]);
    }
    while(1)
    {
        for(int i = 0;i<16;i++)
        {
            PORTD = EEPROM_Read(address[i]);
            __delay_ms(500);
        }
    }
    return;
}
void EEPROM_Write(int address,char data)
{
    while(EEPROM_Busy() == 1);  //Wait for previous Write/Read operations to finish
    SS = 0;
    SPI_Write(WREN);
    SS = 1;     //Latch Enable. 
    SS = 0;
    SPI_Write(WRITE);
    SPI_Write(address>>8);
    SPI_Write(address);
    SPI_Write(data);
    SS = 1;
}
char EEPROM_Read(int address)
{
    while(EEPROM_Busy() == 1);        //Wait for previous Write/Read operations to finish
    SS = 0;
    SPI_Write(READ);
    SPI_Write(address>>8);    //upper byte
    SPI_Write(address);
    char data = SPI_Read(0);
    SS = 1;
    return data;
}
char EEPROM_Status()
{
    SS = 0;
    SPI_Write(RDSR);  //Instruction : Read Status Register of the EEPROM
    char status = SPI_Read(0);
    SS = 1;
    return status;
}
char EEPROM_Busy()
{
    char busy = EEPROM_Status();
    return (busy&0x01);  //LSB is Write-In-Process (WIP) bit. When Set, a write is in progress, when Clear, no write is in progress
}
char EEPROM_wrEnable()
{
    char isWrite = EEPROM_Status();
    return (isWrite&0x02);///Bit 2 is the Write Enable Latch (WEL) bit and indicates the status of the write enable latch
}