/*
 * File:   USART_interrupt.c
 * Author: XUAN
 *
 * Created on January 3, 2022, 8:59 AM
 */


#include <xc.h>
#include "config.h"

#define _XTAL_FREQ 8000000
char data;

void __interrupt() usartISR()
{
    if(RCIF == 1)
    {
        if(RCSTAbits.OERR == 1)
        {
            CREN = 0;
            NOP();
            CREN = 1;
        }
        data = RCREG;
        if(data == 'a')
        {
            for(int i = 0;i<8;i++)
            {
                PORTD = 0x01<<i;
                __delay_ms(500);
            }
        }
        else
        {
            for(int i = 0;i<8;i++)
            {
                PORTD = 0x55;
                __delay_ms(500);
                PORTD = 0xaa;
                __delay_ms(500);
            }
        }
        RCIF = 0;
    }
}

void USART_init(long baud_rate)
{
    float bps;
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;
    bps = (((float)(_XTAL_FREQ/64)/(float)baud_rate)-1);
    SPBRG = (int)bps;
    TXSTAbits.CSRC = 0;
    TXSTAbits.TX9 = 0;
    TXSTAbits.TXEN = 1;
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 0;
    BAUDCONbits.BRG16 = 0;
    RCSTA = 0x90;       //0b10010000
}
void main(void) 
{
    TRISD = 0x00;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    IPR1bits.RCIP = 1;
    PORTD = 0;
    USART_init(9600);
    while(1)
    {
        PORTD = 0;
    }
    return;
}
