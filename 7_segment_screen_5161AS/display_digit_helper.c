#include "display_digit_helper.h"


// cipari sakot no 0
// unit8_t unsigned 8-bit integer type
const uint8_t digit_pins_lookup[10][8] =
{
	1,1,1,1,1,1,0,0,
	0,1,1,0,0,0,0,0,
	1,1,0,1,1,0,1,0,
	1,1,1,1,0,0,1,0,
	0,1,1,0,0,1,1,0,
	1,0,1,1,0,1,1,0,
	1,0,1,1,1,1,1,0,
	1,1,1,0,0,0,0,0,
	1,1,1,1,1,1,1,0,
	1,1,1,1,0,1,1,0
};

// https://orvindemsy.medium.com/bit-operation-in-avr-microcontroller-5979eb5b78fc
// Controlling bit is a way for the programmer to activate/deactivate certain pins in MCU.
void displayDigit(int digit){
	PORTD |= digit_pins_lookup[digit][0]<<PIN_A; //PORTD2
	PORTD |= digit_pins_lookup[digit][1]<<PIN_B;
	PORTD |= digit_pins_lookup[digit][2]<<PIN_C;
	PORTD |= digit_pins_lookup[digit][3]<<PIN_D;
	PORTD |= digit_pins_lookup[digit][4]<<PIN_E;
	PORTD |= digit_pins_lookup[digit][5]<<PIN_F;
	PORTB |= digit_pins_lookup[digit][6]<<PIN_G;
	PORTB |= digit_pins_lookup[digit][7]<<PIN_DP;
}

// ~ NOT maina visus 0 uz 1 un preteeji
// & 1 ja visi ir 1
// ~0x00001000 = 0x11110111
// 0bxxxx1xxx & ~0x11110111 = 0bxxxx0xxx
void turnOff(){
	PORTD &= ~1<<PIN_A;
	PORTD &= ~1<<PIN_B;
	PORTD &= ~1<<PIN_C;
	PORTD &= ~1<<PIN_D;
	PORTD &= ~1<<PIN_E;
	PORTD &= ~1<<PIN_F;
	PORTB &= ~1<<PIN_G;
	PORTB &= ~1<<PIN_DP;
}