/*
 * File:   main.c
 * Author: XUAN
 *
 * Created on December 16, 2021, 12:24 PM
 */


#include <xc.h>
#include "config.h"
#define _XTAL_FREQ 8000000

unsigned char readEEPROM(unsigned char address)
{
    EEADR = address;
    EECON1bits.EEPGD = 0;       //access data EEPROM memory 
    EECON1bits.RD = 1;          //initial read cycle
    return EEDATA;              //return data
}

void writeEEPROM(unsigned char address, unsigned char data)
{
    unsigned char INTCON_SAVE;
    EEADR = address;
    EEDATA = data;
    EECON1bits.EEPGD = 0;       //access data EEPROM memory
    EECON1bits.WREN = 1;        //allow write cycle to data EEPROM
    INTCON_SAVE = INTCON;
    INTCON = 0;                 //disable the interrupt
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;           //initial write cycle
    INTCON = INTCON_SAVE ;      //enable interrupt
    EECON1bits.WREN = 0;        //disable write
    while(PIR2bits.EEIF == 0 );//check for completed of write operation
    PIR2bits.EEIF = 0;          //clear EEIF bit
}
void main(void) 
{
    unsigned int a;
    TRISC = 0x00;
    do
    {
        for(unsigned int i=0,a=1;i<8;i++)
        {
            writeEEPROM(i,a);
            __delay_ms(20);
            a = a<<1;
        }
    
        for(unsigned int i = 0;i<8;i++)
        {
            PORTC = readEEPROM(i);
            __delay_ms(1000);
        }
    }
    while(1);
    
    return;
}
