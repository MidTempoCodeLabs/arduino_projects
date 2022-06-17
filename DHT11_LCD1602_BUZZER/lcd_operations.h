/*
 * lcd_operations.h
 *
 * Created: 04/06/2022 14:25:49
 *  Author: nikita.nikitin
 */ 


/*
* https://funduino.de/DL/1602LCD.pdf  instructions of LCD
* 
*
*/

#define LCD_LINE_ONE 0x00  // start of line 1
#define LCD_LINE_TWO 0x40 // start of line 2  0b01000000

#define LCD_CLEAR			  0b00000001
#define LCD_HOME			  0b00000010		  // returns cursor to first position on first line
#define LCD_ENTRYMODE	      0b00000110          // shift cursor from left to right on read/write
#define LCD_DISPLAY_OFF       0b00001000
#define LCD_DISPLAY_ON        0b00001100          // display on, cursor off, don't blink character
#define LCD_FUNCTION_RESET    0b00110000          // reset the LCD
#define LCD_FUCTION_SET_4_BIT 0b00101000          // 4-bit data, 2-line display, 5 x 7 font
#define LCD_SET_CURSOR        0b10000000          // set cursor position