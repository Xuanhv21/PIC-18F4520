// PIC18F4520 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = ON         // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF. 
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define _XTAL_FREQ 8000000
#define I2C_BaudRate 100000

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
#define LCD_BACKLIGHT 0x08

#define LCD_Startup 15
#define LCD_delay 5

#define NB_LINES 2 // Number of display lines
#define NB_COL 16 // Number of characters per line

#define LCDLine1() LCDPutCmd(LCD_HOME) // legacy support
#define LCDLine2() LCDPutCmd(LCD_CURSOR_LINE2) // legacy support
#define shift_cursor() LCDPutCmd(LCD_CURSOR_FWD) // legacy support
#define cursor_on() LCDPutCmd(LCD_CURSOR_ON) // legacy support
#define DisplayClr() LCDPutCmd(LCD_CLEAR) // Legacy support


unsigned char i2c_add,RS;
unsigned char BackLight_State = LCD_BACKLIGHT;
void I2C_idle()
{
    while((SSPSTAT&0x04)||(SSPCON2&0x1f));
    //0b00000100                   0b00011111
}
void I2C_Master_init()
{
    SSPCON1 = 0x28;      //0b00101000
    SSPCON2 = 0x00;
    SSPSTAT = 0x00;
    SSPADD = ((_XTAL_FREQ/4)/I2C_BaudRate)-1;
    TRISCbits.RC3 = 1;
    TRISCbits.RC4 = 1;
}
void I2C_Start()
{
    I2C_idle();
    SSPCON2bits.SEN = 1;        //initial start condition on SDA line
}
void I2C_Stop()
{
    I2C_idle();
    SSPCON2bits.PEN = 1;        //initial stop condition on SDA and SCL pins
}
void I2C_Restart()
{
    I2C_idle();
    SSPCON2bits.RSEN = 1;       //initiate repeated start condition on SDA and SCL pins
}
void I2C_ACK(void)
{
    I2C_idle();
    SSPCON2bits.ACKDT = 0;      //acknowledge data bit
    SSPCON2bits.ACKEN = 1;      //acknowledge Sequence Enable bit
}
void I2C_NACK(void)
{
    I2C_idle();
    SSPCON2bits.ACKDT = 1;
    SSPCON2bits.ACKEN = 1;
}
unsigned char I2C_Write(unsigned char data)
{
    I2C_idle();
    SSPBUF = data;
    I2C_idle();
    return SSPCON2bits.ACKSTAT;
}
unsigned char I2C_Read(void)
{
    SSPCON2bits.RCEN = 1;   //Enable and Start Reception
    //while(!PIR1bits.SSPIF);
    //PIR1bits.SSPIF = 0;
    while(!SSPSTATbits.BF);
    return SSPBUF;
}

void IO_Expander_Write(unsigned char data)
{
    I2C_Start();
    I2C_Write(i2c_add);
    I2C_Write(data | BackLight_State);
    I2C_Stop();
    __delay_ms(20);
}

void LCDWriteNibble(unsigned char Nibble)
{
    Nibble |= RS;
    IO_Expander_Write(Nibble|0x04);
    IO_Expander_Write(Nibble&0xfb);
    __delay_us(50);
}
void LCDPutCmd(unsigned char ch)
{
    RS = 0;
    __delay_ms(LCD_delay);
    LCDWriteNibble(ch);
    ch = (ch<<4);
    __delay_ms(1);
    LCDWriteNibble(ch);
}

void LCD_initialize(unsigned char I2C_ADD)
{
    i2c_add = I2C_ADD;//      power up LCD
    __delay_ms(LCD_Startup);
    LCDPutCmd(0x32);
    LCDPutCmd(FUNCTION_SET);
    LCDPutCmd(DISPLAY_SETUP);
    DisplayClr();
    LCDPutCmd(ENTRY_MODE);
}

void LCDPutChar(char ch)
{
    RS = 1;
    __delay_ms(LCD_delay);

    //Send higher nibble first
    LCDWriteNibble(ch&0xf0);

    //get the lower nibble
    ch = (ch << 4);

    // Now send the low nibble
    LCDWriteNibble(ch&0xf0);
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
void LCD_Set_Cursor(unsigned char ROW, unsigned char COL)
{
  switch(ROW)
  {
    case 2:
      LCDPutCmd(0xC0 + COL-1);
      break;
    case 3:
      LCDPutCmd(0x94 + COL-1);
      break;
    case 4:
      LCDPutCmd(0xD4 + COL-1);
      break;
    // Case 1
    default:
      LCDPutCmd(0x80 + COL-1);
  }
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

