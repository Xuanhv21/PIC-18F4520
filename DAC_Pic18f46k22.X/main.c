/*
 * File:   main.c
 * Author: XUAN
 *
 * Created on December 10, 2021, 7:42 PM
 */
//#define _XTAL_FREQ 16000000

#include <xc.h>
#include "config.h"

void main(void) 
{   SCS0 = 0;
    SCS1 = 0;

//Frequency select bits
    IRCF0 = 1;
    IRCF1 = 1;
    IRCF2 = 1;
//IRCF3 = 1;

//SET PLLx4 OFF
//SPLLEN = 0;
    VREFCON1bits.DACEN = 1;     //enable the DAC module
    VREFCON1bits.DACLPS = 1;    //active select positive reference voltage
    VREFCON1bits.DACOE = 1;     //enable the DAC voltage output
    VREFCON1bits.DACPSS = 00;   //select a positive voltage source for DAC from Vdd
    VREFCON1bits.DACNSS = 0;    //select a negative voltage source for DAC form Vss
    //Vout = ((VSRC+)-(VSRC-))*(DACR[4:0]/(2^5))+VSRC
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    VREFCON2bits.DACR = 0b0000;
    int i = 0;
    while(1)
    {
        VREFCON2bits.DACR = i;
        i++;
        __delay_us(310);
        if(i>31)
        {
            i=0;
        }
    }
    return;
}
