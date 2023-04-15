/*
 * File:   main_slave.c
 * Author: XUAN
 *
 * Created on December 27, 2021, 8:21 PM
 */


#include <xc.h>
#include "config.h"

#define _XTAL_FREQ 8000000
char data;

void __interrupt() ISR(void)
{
    if(PIR1bits.SSPIF == 1)
    {
        data = SSPBUF;
        PIR1bits.SSPIF = 0;
    }
}

void SPI_init()
{
    //Slave mode,SS enable
    SSPCON1bits.SSPM0 = 1;
    SSPCON1bits.SSPM1 = 0;
    SSPCON1bits.SSPM2 = 1;
    SSPCON1bits.SSPM3 = 0;
    
    SSPCON1bits.SSPEN = 1; //enable the Synchronous Serial Port
    SSPCON1bits.CKP = 0;
    SSPSTATbits.CKE = 0;
    
    SSPCON1bits.WCOL = 0;
    SSPCON1bits.SSPOV = 0;
    SSPSTATbits.SMP = 1; //clear SMP bit
    TRISCbits.RC5 = 0;
    TRISCbits.RC4 = 1;
    TRISCbits.RC3 = 1;
    TRISAbits.RA5 = 1;

}
void main(void) 
{
    SPI_init();
    TRISB = 0X00;
    ei();
    while(1)
    {
        if (data==0x01)     
        {
            PORTBbits.RB0=1;       //if received data = 0x01, turn on LED
        }
        else if (data==0x02)
        {
            PORTBbits.RB0=0;  //if received data = 0x02, turn off LED
        }
    }
    return;
}
