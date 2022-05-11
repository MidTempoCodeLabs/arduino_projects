#include "display_digit_helper.h"
#define F_CPU 16000000UL

void setup(){
	/*setup output pins*/
	// http://www.rjhcoding.com/avrc-bit-manip.php
	// OR ir izmantots jo 0bxxxxxxxx | 0x10000000 = 0b1xxxxxxx
	// DDRD |= 1<<PIN_A = DDRD = DDRD | 1<<PIN_A
	// << ir shifting, tiek izmantots lai uzst?d?t konkr?tu pinu k? output pins
	// PIN_A = PORTD2
	// piem?ram, PIN_A = 2, taapeec 1<<2 = o00000100
	DDRD |= 1<<PIN_A;
	DDRD |= 1<<PIN_B;
	DDRD |= 1<<PIN_C;
	DDRD |= 1<<PIN_D;
	DDRD |= 1<<PIN_E;
	DDRD |= 1<<PIN_F;
	DDRB |= 1<<PIN_G;
	DDRB |= 1<<PIN_DP;
}

int main(){

	setup();
	while(1){
		int i;
		for(i=0;i<10;i++){
			displayDigit(i);
			_delay_ms(10000);
			turnOff();
		}
	}
}