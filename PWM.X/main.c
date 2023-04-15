/*
 * File:   main.c
 * Author: XUAN
 *
 * Created on December 14, 2021, 9:47 PM
 */

#include <xc.h>
#include "config_and_LCD.h"
#define _XTAL_FREQ 8000000
//PWM Period = [(PR2)+1].4Tosc.(TMR2 Prescale Value)
//==>PR2 = (PWM_Period / (4 . Tosc . TMR2 Prescale )) - 1
#define TMR2PRESCALE 4
long frequency;

void PWM1_init(long freq)
{
    PR2 = (_XTAL_FREQ/(freq * 4 *TMR2PRESCALE))-1;
    frequency = freq;
}
void PWM2_init(long freq)
{
    PR2 = (_XTAL_FREQ/(freq * 4 * TMR2PRESCALE))-1;
    frequency = freq;
}
int PWM_max_duty()
{
    return (_XTAL_FREQ/(frequency * TMR2PRESCALE));
}

void PWM1_set_duty(unsigned int duty)       //  thiet lap do rong xung duong PWM1
{
    duty = ((float)duty/100)*1023;
    if(duty<1024)
    {
        duty = ((float)duty/1023)* PWM_max_duty();
        CCP1CONbits.CCP1X = duty & 0x02;
        CCP1CONbits.CCP1Y = duty & 0x01;
        CCPR1L = duty>>0x02;
    }
}
void PWM2_set_duty(unsigned int duty)
{
    duty = ((float)duty/100) * 1023; //percentage;
    if(duty<1024)
    {
        duty = ((float)duty/1023)*PWM_max_duty();
        CCP2CONbits.CCP2X = duty & 0x02;
        CCP2CONbits.CCP2Y = duty & 0x01;
        CCPR2L = duty>>0x02;
    }
}
void PWM1_start()
{
  CCP1CONbits.CCP1M3 = 1;  // select PWM_Mode
  CCP1CONbits.CCP1M2 = 1;  // select PWM_Mode
  
  #if TMR2PRESCALE == 1
    T2CONbits.T2CKPS0 = 0;
    T2CONbits.T2CKPS1 = 0;
  #elif TMR2PRESCALE == 4
    T2CONbits.T2CKPS0 = 1;
    T2CONbits.T2CKPS1 = 0;
  #elif TMR2PRESCALE == 16
    T2CONbits.T2CKPS0 = 1;
    T2CONbits.T2CKPS1 = 1;
  #endif

  T2CONbits.TMR2ON = 1;  // turn_on timer2
  TRISCbits.RC2 = 0;          // Configure RC2 as a digital output
}
void PWM1_stop()
{
  CCP1CONbits.CCP1M3 = 0;  //disable CCP1_PWM_Mode
  CCP1CONbits.CCP1M2 = 0;  //disable CCP1_PWM_Mode
}
void PWM2_start()
{
  CCP2CONbits.CCP2M3 = 1;
  CCP2CONbits.CCP2M2 = 1;
  
   #if TMR2PRESCALE == 1
    T2CKPS0 = 0;
    T2CKPS1 = 0;
  #elif TMR2PRESCALE == 4
    T2CONbits.T2CKPS0 = 1;
    T2CONbits.T2CKPS1 = 0;
  #elif TMR2PRESCALE == 16
    T2CKPS0 = 1;
    T2CKPS1 = 1;
  #endif
  
    T2CONbits.TMR2ON = 1;
    TRISCbits.RC1 = 0;
}

PWM2_stop()
{
  CCP2CONbits.CCP2M3 = 0;
  CCP2CONbits.CCP2M2 = 0;
}



void main(void) 
{
    TRISC=0x00;
    PWM1_init(1000);
    PWM1_start();
    
    //PWM2_init(500);
    //PWM2_start();
    

    PWM1_set_duty(50);
    //PWM2_set_duty(40);
    return;
}
