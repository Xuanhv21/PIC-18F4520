/*
 * File:   main.c
 * Author: XUAN
 *
 * Created on December 20, 2021, 10:04 AM
 */


#include <xc.h>
#include "config.h"
#define _XTAL_FREQ 8000000


void __interrupt() interrupt_0()
{ 
    if(INTCONbits.INT0IF ==1)
    {
        for(int i = 0 ;i<8;i++)
        {
            PORTD = 0x01<<i;
            __delay_ms(300);
        }
        INTCONbits.INT0IF = 0;
    }
} 

void main(void) 
{
    ADCON1 = 0X0f;
    TRISB = 0x03;
    TRISD = 0x00;
    INTCONbits.GIE = 1;         //enable all interrupt
    INTCONbits.INT0IE = 1;      //enable external interrupt 0
    INTCON2bits.INTEDG0 = 0;    //interrupt on falling edge
    ei();
    while(1)
    {
    PORTD = 0x00;
    __delay_ms(500);
    PORTD = 0xff;
    __delay_ms(500);
    }
    return;
}
