/*
 * File:   main_RS232.c
 * Author: XUAN
 *
 * Created on January 13, 2022, 7:01 PM
 */


#include <xc.h>
#include "config.h"

#define _XTAL_FREQ 8000000

void UART_init(long baud_rate)
{
    float bps;
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;
    bps = (((float)(_XTAL_FREQ/64)/(float)baud_rate)-1);
    SPBRG = (int)bps;
    TXSTAbits.CSRC = 0; //Don't care for Synchronous mode
    TXSTAbits.TX9 = 0; //Selects 8 bit data transmission
    TXSTAbits.TXEN = 1; //Enable data transmission on RC6
    TXSTAbits.SYNC = 0; //Select Synchronous Serial Communication Mode
    TXSTAbits.BRGH = 0; //Default baud rate speed
    BAUDCONbits.BRG16 = 0; //Selects only 8 bit register for baud rate
    RCSTA = 0x90; //0b10010000  ,Enable UART communication PORT 
}
void UART_write(char data)
{
    TXREG = data;
    while(TXSTAbits.TRMT == 0);
}
void UART_write_str(char *str)
{
    char i = 0;
    while(str[i])
    {
        UART_write(str[i++]);
    }
}

char UART_read()
{
    while(PIR1bits.RCIF == 0);
    if(RCSTAbits.OERR == 1)
    {
        RCSTAbits.CREN = 0;
        NOP();      //no operation
        RCSTAbits.CREN = 1;
    }
    return (RCREG);
}
void main(void) 
{
    //OSCCON = 0x72 ; //0b01110010
    TRISB = 0X00;
    UART_init(19200);
    while(1)
    {
        UART_write('X');
        UART_write('U');
        UART_write('A');
        UART_write('N');
        UART_write('\r');
        UART_write_str("HV");
        UART_write('\r');
        __delay_ms(500);
    }
    return;
}
