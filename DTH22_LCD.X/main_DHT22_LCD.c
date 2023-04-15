/*
 * File:   main_DTH22_LCD.c
 * Author: XUAN
 *
 * Created on January 22, 2022, 6:05 PM
 */


#include <xc.h>
#include "config.h"


#define Data PORTAbits.RA0
#define DataDir TRISAbits.RA0

char message1[] = "Temp = 00.0 C";
char message2[] = "Humidity = 00.0 %";
unsigned short TOUT = 0,CheckSum,i;
unsigned short T_Byte1,T_Byte2,H_Byte1,H_Byte2;

void StartSignal()
{
    DataDir = 0;
    Data = 0;
    __delay_ms(18);
    DataDir = 1;
    __delay_us(80);
}

unsigned short CheckResponse()
{
    TOUT = 0;
    TMR2 = 0;
    TMR2ON = 1;
    while(!Data && !TOUT);
    if(TOUT)
    {
        return 0;
    }
    else
    {
        TMR2 = 0;
        while(Data &&!TOUT);
        if(TOUT)
        {
            return 0;
        }
        else
        {
            TMR2ON = 0;
            return 1;
        }
    }
}

unsigned short ReadByte()
{
    unsigned short num = 0;
    DataDir = 1;
    for(int i=0;i<8;i++)
    {
        while(!Data);
        TMR2 = 0;
        TMR2ON = 1;
        while(Data);
        TMR2ON = 0;
        if(TMR2>40)
        {
            num |= 1<<(7-i);
        }
    }
    return num;
}
void __interrupt() tc_init(void)
{
    if(TMR2IF)
    {
        TOUT = 1;
        TMR2ON = 0;         //Stop Timer
        TMR2IF = 0;         //Clear Tmr0 Interrupt Flag
    }
}

void main(void) 
{
    unsigned short check,sign;
    unsigned int rh,temp;
    GIE = 1;
    PEIE = 1;
    TMR2IE = 1;
    T2CON = 0;
    TMR2IF = 0;
    TMR2 = 0;
    LCD_initialize();
    DisplayClr();
    while(1)
    {
        __delay_ms(1000);
        StartSignal();
        check = CheckResponse();
        if(!check)
        {
            DisplayClr();
            LCDGoto(0,0);
            LCDPutStr("No Response");
            LCDGoto(0,1);
            LCDPutStr("from the sensor");
        }
        else
        {
            H_Byte1 = ReadByte();
            H_Byte2 = ReadByte();
            T_Byte1 = ReadByte();
            T_Byte2 = ReadByte();
            CheckSum = ReadByte();
            rh = H_Byte2 | (H_Byte1<<8);
            temp = T_Byte2 |(T_Byte1<<8);
            sign = 0;
            if(temp > 0x8000)
            {
                temp &= 0x0fff;
                sign = 1;
            }
            unsigned char h_buf[5],temp_buf[5];
            sprintf(h_buf,"%03u",rh);
            sprintf(temp_buf,"%03u",temp);
            if(CheckSum == (H_Byte1 + H_Byte2 + T_Byte1 + T_Byte2)& 0xff)
            {
                message2[11] = h_buf[0];
                message2[12] = h_buf[1];
                message2[14] = h_buf[2];
                if(sign)
                {
                    message1[6] = '-';
                }
                else
                {
                    message1[6] = ' ';
                    message1[7] = temp_buf[0];
                    message1[8] = temp_buf[1];
                    message1[10] = temp_buf[2];
                    message1[11] = 223; //symbol Degree
                    DisplayClr();
                    LCDGoto(0,0);
                    LCDPutStr(message1);
                    LCDGoto(0,1);
                    LCDPutStr(message2);
                }
            }
            else
            {
                DisplayClr();
                LCDGoto(0,0);
                LCDPutStr("Checksum Error !");
                LCDGoto(0,1);
                LCDPutStr("Trying again ...");
            }
        }
    }
    return;
}
