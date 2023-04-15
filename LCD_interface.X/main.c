/*
 * File:   main.c
 * Author: XUAN
 *
 * Created on December 14, 2021, 10:41 AM
 */
#include <xc.h>
#include "LCD.h"

void main(void) 
{
    LCD_initialize();
    LCDPutStr(" Hello World!"); //Display String "Hello World"
    LCDGoto(8,1); //Go to column 8 of second line
    LCDPutStr("Start"); //Display character '1'
    DisplayClr(); // Clear the display
    LCDGoto(2,0);
    LCDPutStr(" LCD Display"); // Dispay a string "LCD Display"
    LCDGoto(2,1); //Go to second line 
    LCDPutStr("HoangVinhXuan"); //Display String "Micro Lab" 
    while (1)
    {
    // Add your application code
    }
    return;
}
