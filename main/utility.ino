#include <SRIO_mxctr.h>
#include "din.h"
/*
 * contains macros common to MIOS8 and some coding hints
 * */
 
// cool functions :)
// https://github.com/paradajz/OpenDeck/blob/master/src/Firmware/core/helpers/BitManipulation.h
//
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define invertByte(value) ((value) ^ 0xFF)

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
void DOUT_PinSet(unsigned char dout_pin ,  unsigned char dout_pin_state)
{
  SR.Led_Pin_Write(dout_pin, dout_pin_state);
}

////////////////////////////////////////////////////////////////////
void DOUT_PinSet0(unsigned char dout_pin)
{
  SR.Led_Pin_Write(dout_pin, 0);
}

////////////////////////////////////////////////////////////////////
void DOUT_PinSet1(unsigned char dout_pin)
{
  SR.Led_Pin_Write(dout_pin, 1);
}

////////////////////////////////////////////////////////////////////
void DOUT_PinSetOff(unsigned char dout_pin)
{
  SR.Led_Pin_Write(dout_pin, 0);
}

////////////////////////////////////////////////////////////////////
void DOUT_PinSetOn(unsigned char dout_pin)
{
  SR.Led_Pin_Write(dout_pin, 1);
}

////////////////////////////////////////////////////////////////////
void DOUT_PinSet_Keypanel(unsigned char key1_led, unsigned char key2_led, unsigned char key3_led, unsigned char key4_led, unsigned char key5_led, unsigned char key_page_led)
{
  SR.Led_Pin_Write(DOUT_DCO, key1_led);
  SR.Led_Pin_Write(DOUT_FILTER, key2_led);
  SR.Led_Pin_Write(DOUT_ENV, key3_led);
  SR.Led_Pin_Write(DOUT_KBD, key4_led);
  SR.Led_Pin_Write(DOUT_MATRIXMOD, key5_led);
  SR.Led_Pin_Write(DOUT_PAGE, key_page_led);
}

////////////////////////////////////////////////////////////////////
unsigned char DOUT_PinGet(unsigned char pin)
{
  return SR.Led_Pin_Read(pin);
}

////////////////////////////////////////////////////////////////////
void LCD_CursorSet(unsigned char cell)
{
  //sets the text cursor on LCD screen
  //0x00-0x3f: first line
  //0x40-0x7f: second line

  if (cell < LCD_SECONDROWSTARTPOSITION)
    lcd.setCursor(cell, 0);
  else
    lcd.setCursor(cell - LCD_SECONDROWSTARTPOSITION, 1);
}


////////////////////////////////////////////////////////////////////
void LCD_Clear(void)
{
  lcd.clear();
}

////////////////////////////////////////////////////////////////////
void LCD_PrintBCD3(int value)
{
  LCD_PrintBCD(value, 3);
}


////////////////////////////////////////////////////////////////////
void LCD_PrintBCD2(int value)
{
  LCD_PrintBCD(value, 2);
}

int intlen(int n) {
    if (n < 0) n = (n == INT8_MIN) ? INT8_MAX : -n;
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;
    return 10;
}


void LCD_PrintBCD(int value, unsigned int width)
{
  for (int i = intlen(value); i < width; i++)
    lcd.print(" ");
  lcd.print(value, DEC);
}

void LCD_PrintBCDPrepend(char *prepend, int value, unsigned int width)
{
  lcd.print(prepend);
  LCD_PrintBCD(value, width);
}


////////////////////////////////////////////////////////////////////
void LCD_PrintBCD1(int value)
{
  LCD_PrintBCD(value, 1);
}


////////////////////////////////////////////////////////////////////
void LCD_PrintChar(char caractere)
{
  lcd.print(caractere);
}


////////////////////////////////////////////////////////////////////
void LCD_PrintCString( String msg)
{
  lcd.print (msg);
}

////////////////////////////////////////////////////////////////////
void LCD_PrintHex2(unsigned char data)
{
  lcd.print (data, HEX);
}

void LCD_setAndPrint(unsigned char x, unsigned char y, String msg)
{
  lcd.setCursor(x, y);
  lcd.print(msg);
}
