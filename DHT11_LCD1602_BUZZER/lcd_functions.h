/*
 * lcd_functions.h
 *
 * Created: 04/06/2022 14:31:30
 *  Author: nikita.nikitin
 */ 

#include <avr/io.h>

void lcd_init_4d(void);
void lcd_clear_screen();
void lcd_write_instruction_4d(uint8_t);
void lcd_write_4(uint8_t);
void lcd_write_string_4d(uint8_t *);
void lcd_write_character_4d(uint8_t);
void displayTextTwoLines(uint8_t firstLineText[], uint8_t secondLineText[]);