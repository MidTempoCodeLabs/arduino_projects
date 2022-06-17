/*
 * lcd_functions.c
 *
 * Created: 04/06/2022 14:35:58
 *  Author: nikita.nikitin
 */ 
#define F_CPU 16000000UL
#include <util/delay.h>
#include "lcd_functions.h"
#include "lcd_operations.h"
#include "port_definitions.h"
#include "bit_definitions.h"

void lcd_init_4d(void)
{
	// Power-up delay
	_delay_ms(100);
	
	// LCD is wired for the 4-bit mode, thus other 4 are left open.
	// SET up the RS and E lines for the 'lcd_write_4'
	LCD_REGISTER_SELECT_PORT &= ~(1<<LCD_REGISTER_SELECT_BIT); // select the Instruction Register (RS low)
	LCD_ENABLE_MODULE_PORT &= ~(1<<LCD_ENABLE_MODULE_BIT);	   // make sure E is initially low
	
	// RESET the LCD controller
	lcd_write_4(LCD_FUNCTION_RESET);                 
	_delay_ms(10);                                  

	lcd_write_4(LCD_FUNCTION_RESET);                 
	_delay_us(200);                                 

	lcd_write_4(LCD_FUNCTION_RESET);                 
	_delay_us(200);        
	
	lcd_write_4(LCD_FUCTION_SET_4_BIT);              
	_delay_us(80);
	
	lcd_write_instruction_4d(LCD_FUCTION_SET_4_BIT); 
	_delay_us(80);  
	
	lcd_clear_screen();                                                
}

void lcd_clear_screen(){
	// Display On/Off Control instruction
	lcd_write_instruction_4d(LCD_DISPLAY_OFF);        // turn display OFF
	_delay_us(80);

	// Clear Display instruction
	lcd_write_instruction_4d(LCD_CLEAR);             // clear display RAM
	_delay_ms(4);

	// ; Entry Mode Set instruction
	lcd_write_instruction_4d(LCD_ENTRYMODE);         // set desired shift characteristics
	_delay_us(80);
	
	// Display On/Off Control instruction
	lcd_write_instruction_4d(LCD_DISPLAY_ON);         // turn the display ON
	_delay_us(80);
}

void lcd_write_string_4d(uint8_t theString[])
{
	volatile int i = 0;		// variable should be declared volatile whenever its value could change unexpectedly.                            
	while (theString[i] != 0)
	{
		lcd_write_character_4d(theString[i]);
		i++;
		_delay_us(80);                           
	}
}

void lcd_write_character_4d(uint8_t theData)
{
	LCD_REGISTER_SELECT_PORT |= (1<<LCD_REGISTER_SELECT_BIT);                 // select the Data Register (RS high)
	LCD_ENABLE_MODULE_PORT &= ~(1<<LCD_ENABLE_MODULE_BIT);					  // make sure E is initially low
	lcd_write_4(theData);                           
	lcd_write_4(theData << 4);                      
}

void lcd_write_instruction_4d(uint8_t theInstruction)
{
	LCD_REGISTER_SELECT_PORT &= ~(1<<LCD_REGISTER_SELECT_BIT);                // select the Instruction Register (RS low)
	LCD_ENABLE_MODULE_PORT &= ~(1<<LCD_ENABLE_MODULE_BIT);                  // make sure E is initially low
	lcd_write_4(theInstruction);                    
	lcd_write_4(theInstruction << 4);               
}

void lcd_write_4(uint8_t theByte)
{
	LCD_DATA_BUS_LINE_7_PORT &= ~(1<<LCD_DATA_BUS_LINE_7_BIT);                        // assume that data is '0'
	if (theByte & 1<<7) LCD_DATA_BUS_LINE_7_PORT |= (1<<LCD_DATA_BUS_LINE_7_BIT);     // make data = '1' if necessary

	LCD_DATA_BUS_LINE_6_PORT &= ~(1<<LCD_DATA_BUS_LINE_6_BIT);                        // repeat for each data bit
	if (theByte & 1<<6) LCD_DATA_BUS_LINE_6_PORT |= (1<<LCD_DATA_BUS_LINE_6_BIT);

	LCD_DATA_BUS_LINE_5_PORT &= ~(1<<LCD_DATA_BUS_LINE_5_BIT);
	if (theByte & 1<<5) LCD_DATA_BUS_LINE_5_PORT |= (1<<LCD_DATA_BUS_LINE_5_BIT);

	LCD_DATA_BUS_LINE_4_PORT &= ~(1<<LCD_DATA_BUS_LINE_4_BIT);
	if (theByte & 1<<4) LCD_DATA_BUS_LINE_4_PORT |= (1<<LCD_DATA_BUS_LINE_4_BIT);

	// write the data
	// 'Address set-up time' (40 nS)
	LCD_ENABLE_MODULE_PORT |= (1<<LCD_ENABLE_MODULE_BIT);       // Enable pin high
	_delay_us(1);												// implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)
	LCD_ENABLE_MODULE_PORT &= ~(1<<LCD_ENABLE_MODULE_BIT);      // Enable pin low
	_delay_us(1);												// implement 'Data hold time' (10 nS) and 'Enable cycle time' (500 nS)
}

void displayTextTwoLines(uint8_t firstLineText[], uint8_t secondLineText[]) {
	lcd_write_string_4d(firstLineText);
	lcd_write_instruction_4d(LCD_SET_CURSOR | LCD_LINE_TWO);
	_delay_us(80);
	lcd_write_string_4d(secondLineText);
}