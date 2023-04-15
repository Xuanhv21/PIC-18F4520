
#include<xc.h>
#include<pic18f4520.h>
#include"config.h"

#define _XTAL_FREQ 4000000

#define LED PORTBbits.RB1
#define BT PORTBbits.RB0 
void Led_Button()
{
    if(BT ==1)
        {
            LED = 1;
        }
    else
        {
            LED = 0;
        }
}
#define RELAY PORTBbits.RB7
#define BT2 PORTBbits.RB6
void Motor_Relay()
{
    if(BT2 == 1)
    {
        RELAY = 1;
    }
    else
    {
        RELAY = 0;
    }
}
#define clock PORTCbits.RC0
#define data PORTCbits.RC1
#define latch PORTCbits.RC2
unsigned char dem[10] = {0b11111101,//0
0b00001101,//1
0b11011011,//2
0b11110011,//3
0b01100111,//4
0b10110111,//5
0b10111111,//6
0b11100001,//7
0b11111111,//8
0b11110111 //9
};
void clock_signal()
{
    clock = 1;
    __delay_us(500);
    clock = 0;
}
void latch_enable()
{
    latch = 1;
    __delay_us(500);
    latch = 0;
}
void DisplayLed7Seg(unsigned char dl)//1 led 7 doan
{ 
    for(int i=0;i<8;i++)
    {
        data = dl>>i;
        clock_signal();
    }
    latch_enable();
}

void Display2_led_7Seg(unsigned char dl1,unsigned char dl2)   //2 led 7 doan
{
    for(int i=0;i<8;i++)
    {
        data = dl2>>i;
        clock_signal();
    }
    for(int i=0;i<8;i++)
    {
        data = dl1>>i;
        clock_signal();
    }
    latch_enable();
}

int main() 
{ 
    TRISB = 0b01000001;
    TRISC = 0x00; 
    while(1)
    {
        Led_Button();
        Motor_Relay();
        
        for(int i=0;i<100;i++)
        {
            //DisplayLed7Seg(dem[i]);
            Display2_led_7Seg(dem[i/10],dem[i%10]);
            __delay_ms(500);
        }
    }
    
    
} 
