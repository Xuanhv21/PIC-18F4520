/*
 * File:   main_I2C_LCD.c
 * Author: XUAN
 *
 * Created on January 8, 2022, 6:22 PM
 */


#include <xc.h>
#include "I2C_LCD_Sample.h"

/*void main(void) 
{
    I2C_Master_init();
    LCD_initialize(0x4E);
    LCD_Set_Cursor(1,1); //Go to column 8 of second line
    LCDPutStr("Start"); //Display character '1'
    DisplayClr(); // Clear the display
    LCD_Set_Cursor(2,1);
    LCDPutStr(" LCD Display"); // Dispay a string "LCD Display"
    return;
}*/

void main(void) {
 
  I2C_Master_Init();
  LCD_Init(0x4E); // Initialize LCD module with I2C address = 0x4E
 
  LCD_Set_Cursor(1, 1);
  LCD_Write_String("HOANGVINHXUAN");
  LCD_Set_Cursor(2, 1);
  LCD_Write_String(" ");
 
  while(1)
  {
 
  }
  return;
}
