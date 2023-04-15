  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define _XTAL_FREQ 8000000

#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_CURSOR_BACK 0x10
#define LCD_CURSOR_FWD 0x14
#define LCD_PAN_LEFT 0x18
#define LCD_PAN_RIGHT 0x1C
#define LCD_CURSOR_OFF 0x0C
#define LCD_CURSOR_ON 0x0E
#define LCD_CURSOR_BLINK 0x0F
#define LCD_CURSOR_LINE2 0xC0
#define FUNCTION_SET 0x28
#define ENTRY_MODE 0x06
#define DISPLAY_SETUP 0x0C

#define LCD_PORT PORTD
#define LCD_PWR PORTDbits.RD7
#define LCD_EN PORTDbits.RD6
#define LCD_RW PORTDbits.RD5
#define LCD_RS PORTDbits.RD4

#define LCD_Startup 15
#define LCD_delay 5


#define LCDLine1() LCDPutCmd(LCD_HOME) // legacy support
#define LCDLine2() LCDPutCmd(LCD_CURSOR_LINE2) // legacy support
#define shift_cursor() LCDPutCmd(LCD_CURSOR_FWD) // legacy support
#define cursor_on() LCDPutCmd(LCD_CURSOR_ON) // legacy support
#define DisplayClr() LCDPutCmd(LCD_CLEAR) // Legacy support

#define instr 0
#define data 1


#define NB_LINES 2 // Number of display lines
#define NB_COL 16 // Number of characters per line

void LCDWriteNibble(char ch , char rs)
{
    ch = (ch>>4);
    ch = (ch& 0x0F);
    LCD_PORT = (LCD_PORT & 0XF0);
    LCD_PORT = (LCD_PORT | ch); 
    LCD_RS = rs;
    LCD_RW = 0;
    LCD_EN = 1;
    LCD_EN = 0;
}
void LCDPutCmd(char ch)
{
    __delay_ms(LCD_delay);
    LCDWriteNibble(ch, instr);
    ch = (ch<<4);
    __delay_ms(1);
    LCDWriteNibble(ch, instr);
}
// LCD initialization Function
void LCD_initialize()
{
    LCD_PORT = 0;
    TRISD = 0X00;
    LCD_PWR = 1;//      power up LCD
    __delay_ms(LCD_Startup);
    LCDPutCmd(0x32);
    LCDPutCmd(FUNCTION_SET);
    LCDPutCmd(DISPLAY_SETUP);
    DisplayClr();
    LCDPutCmd(ENTRY_MODE);
}

void LCDPutChar(char ch)
{
    __delay_ms(LCD_delay);

    //Send higher nibble first
    LCDWriteNibble(ch,data);

    //get the lower nibble
    ch = (ch << 4);

    // Now send the low nibble
    LCDWriteNibble(ch,data);
}

void LCDPutStr(const char *str)
{
    char i=0;

    // While string has not been fully traveresed
    while (str[i])
    {
    // Go display current char
    LCDPutChar(str[i++]);
    }
}
    
void LCDGoto(char pos,char ln)
{
    // if incorrect line or column
    if ((ln > (NB_LINES-1)) || (pos > (NB_COL-1)))
    {
    // Just do nothing
    return;
    }

    // LCD_Goto command
    LCDPutCmd((ln == 1) ? (0xC0 | pos) : (0x80 | pos));

    // Wait for the LCD to finish
    __delay_ms(LCD_delay);
}

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

