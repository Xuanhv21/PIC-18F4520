/*
 * File:   main.c
 * Author: XUAN
 *
 * Created on December 27, 2021, 8:06 PM
 */


#include <xc.h>
#include "config.h"
void SPI_init()
{
    //frequency Fosc/4, Master mode
    SSPCON1bits.SSPM0 = 0;
    SSPCON1bits.SSPM1 = 0;
    SSPCON1bits.SSPM2 = 0;
    SSPCON1bits.SSPM3 = 0;
    
    SSPCON1bits.SSPEN = 1; //enable SPI Port
    //config the clock Polarity and Phase
    SSPCON1bits.CKP = 0;
    SSPSTATbits.CKE = 0;
    
    SSPSTATbits.SMP = 0;        //Slew rate control enable for High Speed mode
    SSPCON1bits.WCOL = 0;
    SSPCON1bits.SSPOV = 0;
    TRISCbits.RC5 = 0;
    TRISCbits.RC4 = 1;
    TRISCbits.RC3 = 0;

}

void SPI_write(char data)
{
    SSPBUF = data;
    while(SSPSTATbits.BF ==0);
}

void main(void) 
{
    SPI_init();
    TRISB = 0b00000001;
    while(1)
    {
        if(PORTBbits.RB0==1)
        {
            SPI_write(0x01);
        }
        else
        {
            SPI_write(0x02);
        }
    }
    return;
}
