/*
 * File:   main_led_matrix_hc595.c
 * Author: XUAN
 *
 * Created on December 10, 2021, 1:52 PM
 */

#include "config.h"
#include <xc.h>
#define _XTAL_FREQ 4000000

int row[] = {1, 2, 4, 8, 16, 32, 64, 128};
int column[] = {128, 64, 32, 16, 8, 4, 2, 1};
unsigned int Kytu[] = {0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x66};
int characterHEX[][8] = {
{0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x66},//A
{0x78,0x64,0x68,0x78,0x64,0x66,0x66,0x7C},//B
{0x3C,0x62,0x60,0x60,0x60,0x62,0x62,0x3C},//C
{0x78,0x64,0x66,0x66,0x66,0x66,0x64,0x78},//D
{0x7E,0x60,0x60,0x7C,0x60,0x60,0x60,0x7E},//E
{0x7E,0x60,0x60,0x7C,0x60,0x60,0x60,0x60},//F
{0x3C,0x62,0x60,0x60,0x66,0x62,0x62,0x3C},//G
{0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x66},//H
{0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x7E},//I
{0x7E,0x18,0x18,0x18,0x18,0x18,0x1A,0x0C},//J
{0x62,0x64,0x68,0x70,0x70,0x68,0x64,0x62},//K
{0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x7E},//L
{0xC3,0xE7,0xDB,0xDB,0xC3,0xC3,0xC3,0xC3},//M
{0x62,0x62,0x52,0x52,0x4A,0x4A,0x46,0x46},//N
{0x3C,0x66,0x66,0x66,0x66,0x66,0x66,0x3C},//O
{0x7C,0x62,0x62,0x7C,0x60,0x60,0x60,0x60},//P
{0x38,0x64,0x64,0x64,0x64,0x6C,0x64,0x3A},//Q
{0x7C,0x62,0x62,0x7C,0x70,0x68,0x64,0x62},//R
{0x1C,0x22,0x30,0x18,0x0C,0x46,0x46,0x3C},//S
{0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x18},//T
{0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x3C},//U
{0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x18},//V
{0x81,0x81,0x81,0x81,0x81,0x99,0x99,0x66},//W
{0x42,0x42,0x24,0x18,0x18,0x24,0x42,0x42},//X
{0xC3,0x66,0x3C,0x18,0x18,0x18,0x18,0x18},//Y
{0x7E,0x02,0x04,0x08,0x10,0x20,0x40,0x7E},//Z
{0x3C,0x66,0x66,0x6E,0x76,0x66,0x66,0x3C},//0
{0x18,0x38,0x58,0x18,0x18,0x18,0x18,0x7E},//1
{0x3C,0x66,0x66,0x0C,0x18,0x30,0x7E,0x7E},//2
{0x7E,0x0C,0x18,0x3C,0x06,0x06,0x46,0x3C},//3
{0x0C,0x18,0x30,0x6C,0x6C,0x7E,0x0C,0x0C},//4
{0x7E,0x60,0x60,0x7C,0x06,0x06,0x46,0x3C},//5
{0x04,0x08,0x10,0x38,0x6C,0x66,0x66,0x3C},//6
{0x7E,0x46,0x0C,0x18,0x18,0x18,0x18,0x18},//7
{0x3C,0x66,0x66,0x3C,0x66,0x66,0x66,0x3C},//8
{0x3C,0x66,0x66,0x36,0x1C,0x08,0x10,0x20},//9
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},// kho?ng tr?ng
{0x00,0x66,0xFF,0xFF,0x7E,0x3C,0x18,0x00}// h�nh tr�i tim, k� hi?u l� '&'
};
char character[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
'P','Q','R','S','T','U','V','W','X','Y','Z','0',
'1','2','3','4','5','6','7','8','9',' ','&'};
#define clock_pin PORTCbits.RC0
#define data_pin PORTCbits.RC1
#define latch_pin PORTCbits.RC2

void clock_signal()
{
    clock_pin = 1;
    __delay_us(5);
    clock_pin = 0;
}
void latch_signal()
{
    latch_pin = 1;
    __delay_us(5);
    latch_pin = 0;
}
void DisplayLedMatrix()
{
    for(int i = 0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            data_pin = (~characterHEX[1][i])>>j;
            clock_signal();
        }
        for(int j=0;j<8;j++)
        {
            data_pin = row[i]>>j;
            clock_signal();
        }
        latch_signal();
        __delay_us(5);
    }
}

void DisplayCharacter(char kt)
{
    for(int i=0;i<38;i++)
    {
        if(kt == character[i])
        {
            for(int k = 0;k<8;k++)
            {
                for(int j=0;j<8;j++)
                {
                    data_pin = (~characterHEX[i][k])>>j;
                    clock_signal();
            }
                for(int j=0;j<8;j++)
            {
                data_pin = row[k]>>j;
                clock_signal();
            }
            latch_signal();
            __delay_us(5);
    }
        }
    }
}

void main(void) {
    TRISC = 0X00;
    while(1)
    {
        //DisplayLedMatrix();
        DisplayCharacter('F');
    }
}
