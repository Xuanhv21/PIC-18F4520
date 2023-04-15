/*
 * File:   main.c
 * Author: XUAN
 *
 * Created on December 18, 2021, 9:39 PM
 */

#include "config.h"
#include <xc.h>
#define _XTAL_FREQ 8000000

void main(void) 
{
    T0CON = 0b10010011;     //che do 16bit,prescale = 1:16
    TRISD = 0X00;
    PORTD = 0x01;
    while(1)
    {
        T0CONbits.TMR0ON = 0;//dung timer
        TMR0L = (65535-62500)%256;
        TMR0H = (65535-62500)/256;
        T0CONbits.TMR0ON = 1;
        while(INTCONbits.TMR0IF==0);
        INTCONbits.TMR0IF = 0;
        PORTD = (PORTD<<1)|(PORTD>>7);
        
    }
    return;
}
