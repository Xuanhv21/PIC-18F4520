/*
 * File:   main_I2C.c
 * Author: XUAN
 *
 * Created on January 3, 2022, 7:33 PM
 */


#include <xc.h>
#include "config.h"
#define _XTAL_FREQ 8000000
#define I2C_BaudRate 100000
void I2C_idle()
{
    while((SSPSTAT&0x04)||(SSPCON2&0x1f));
    //0b00000100                   0b00011111
}
void I2C_Master_init()
{
    SSPCON1 = 0x28;      //0b00101000
    SSPCON2 = 0x00;
    SSPSTAT = 0x00;
    SSPADD = ((_XTAL_FREQ/4)/I2C_BaudRate)-1;
    TRISCbits.RC3 = 1;
    TRISCbits.RC4 = 1;
}
void I2C_Start()
{
    I2C_idle();
    SSPCON2bits.SEN = 1;        //initial start condition on SDA line
}
void I2C_Stop()
{
    I2C_idle();
    SSPCON2bits.PEN = 1;        //initial stop condition on SDA and SCL pins
}
void I2C_Restart()
{
    I2C_idle();
    SSPCON2bits.RSEN = 1;       //initiate repeated start condition on SDA and SCL pins
}
void I2C_ACK(void)
{
    I2C_idle();
    SSPCON2bits.ACKDT = 0;      //acknowledge data bit
    SSPCON2bits.ACKEN = 1;      //acknowledge Sequence Enable bit
}
void I2C_NACK(void)
{
    I2C_idle();
    SSPCON2bits.ACKDT = 1;
    SSPCON2bits.ACKEN = 1;
}
unsigned char I2C_Write(unsigned char data)
{
    I2C_idle();
    SSPBUF = data;
    I2C_idle();
    return SSPCON2bits.ACKSTAT;
}
unsigned char I2C_Read(void)
{
    SSPCON2bits.RCEN = 1;   //Enable and Start Reception
    //while(!PIR1bits.SSPIF);
    //PIR1bits.SSPIF = 0;
    while(!SSPSTATbits.BF);
    return SSPBUF;
}
void WriteEEPROM(unsigned int address,unsigned int data)
{
    unsigned int temp;
    I2C_Start();
    I2C_Write(0xa2);
    temp = address>>8;
    I2C_Write(temp);
    I2C_Write(address);
    I2C_Write(data);
    //I2C_ACK();
    I2C_Stop();
    __delay_ms(20);
}
unsigned int ReadEEPROM(unsigned int address)
{
    unsigned int temp;
    I2C_Start();
    I2C_Write(0xa2);//0b10100000
    temp = address>>8;
    I2C_Write(temp);
    I2C_Write(address);
    I2C_Restart();
    I2C_Write(0xa3);
    temp = I2C_Read();
    I2C_NACK();
    I2C_Stop();
    return temp;
}
void main(void) 
{
    unsigned int a,i;
    TRISB = 0;
    PORTB = 0;
    ADCON1 = 0x06;
    I2C_Master_init();
    for(i=0,a=3;i<8;i++)
    {
        WriteEEPROM(i,a);
        __delay_ms(30);
        a = a<<1;
    }
    while(1)
    {
        for(i=0;i<8;i++)
        {
            PORTB = ReadEEPROM(i);
            __delay_ms(200);
        }
    }
    
    return;
}
