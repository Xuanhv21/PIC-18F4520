/*
 * File:   main.c
 * Author: XUAN
 *
 * Created on December 21, 2021, 10:47 AM
 */


#include <xc.h>
#include "config_and_LCD.h"
#include <string.h>
#include <math.h>
long t2= 0;
double MPH;
int i = 0;
int doub;
char periodData[20];
char data[7];
void hardwareInit()
{
    CCP1CON = 0x05;
    CCP2CON = 0x04;
    TRISCbits.RC1 = 1;
    TRISCbits.RC2 = 1;
    PIE1bits.CCP1IE = 1;    //Enable CCP1 interrupt CCP1
    PIE2bits.CCP2IE = 1;    //Enable CCP2 interrupt CCP2
    PIR1bits.TMR1IF = 0;    // enable timer1 overflow
    INTCONbits.PEIE = 1;    //enable global and peripheral interrupt
    INTCONbits.GIE = 1;
    T1CON = 0x00;
    T1CONbits.TMR1ON = 0;   //turn off timer 1
    PIE1bits.TMR2IE = 1;
}
void __interrupt() INTERRUPT()
{
    if(PIR1bits.CCP1IF == 1)
    {
        T1CONbits.TMR1ON = 1;       //turn on timer1
        PIE1bits.TMR2IE = 1;
        PIR1bits.CCP1IF = 0;        //reset flag bit
        PIE1bits.CCP1IE = 0;        //
    }
    if(PIR2bits.CCP2IF == 1)
    {
        PIE2bits.CCP2IE = 0;
        T1CONbits.TMR1ON = 0;
        PIE1bits.TMR2IE = 0;
        t2 = t2 + (TMR1H<<8)|(TMR1L);
        MPH = (5007999/t2);
        TMR1L = 0x00;
        TMR1H = 0x00;
        PIR2bits.CCP2IF = 0;
        PIE2bits.CCP2IE = 1;
        PIE1bits.CCP1IE = 1;
        t2 = 0;
    }
    if(PIR1bits.TMR1IF == 1)
    {
        t2 = t2 + 1023;
        TMR1H = 0x00;
        TMR1L = 0x00;
        PIR1bits.TMR1IF = 0;
        i++;
        T1CONbits.TMR1ON = 1;
    }
}
void main(void) 
{
    LCD_Init();
    LCD_String_xy(1,0,"Period = ");
    hardwareInit();
    T1CONbits.TMR1ON = 0;
    while(1)
    {
        doub = floor(1/MPH * 1000);
        sprintf(periodData,"%d",doub);
        sprintf(data,"%d",i);
        strcat(periodData," m s");
        LCD_String_xy(1,8,periodData);
        LCD_String_xy(2,5,data);
    }
    return;
}
