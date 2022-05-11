/*
 * blink_2_LEDs.c
 *
 * Created: 5/11/2022 9:40:54 PM
 * Author : nikita.nikitin
 */ 

// PB2
// PB3

#include <avr/io.h>

#define F_CPU 16000000
#define BLINK_DELAY_MS 500

#include <util/delay.h>

#define YELLOW_LED_PIN 10
#define BLUE_LED_PIN 11

const int arduino_port_lookup[14] =
{
	PORTD0, PORTD1, PORTD2, PORTD3, PORTD4,
	PORTD5, PORTD6, PORTD7, PORTB0, PORTB1,
	PORTB2, PORTB3, PORTB4, PORTB5
};

#define YELLOW_LED_PORT arduino_port_lookup[YELLOW_LED_PIN]
#define BLUE_LED_PORT arduino_port_lookup[BLUE_LED_PIN]

void setup() {
	DDRB |= 1<<YELLOW_LED_PORT;
	DDRB |= 1<<BLUE_LED_PORT;
}

void activateLed(int portsOutput[]) {
	PORTB |= portsOutput[0]<<YELLOW_LED_PORT;
	PORTB |= portsOutput[1]<<BLUE_LED_PORT;
}

void turnLedsOff() {
	PORTB &= ~1<<YELLOW_LED_PORT;
	PORTB &= ~1<<BLUE_LED_PORT;
}

void activateOneLedConsequentlyAndAllLEDsPeriodically(
		int mainLEDUsualValue, 
		int secondaryLEDUsualValue, 
		int numberOfRepetitions,
		int period) {
			
		for(int i=0; i<numberOfRepetitions; i++) {
			activateLed((int[]){mainLEDUsualValue, secondaryLEDUsualValue});
			_delay_ms(BLINK_DELAY_MS/5);
			turnLedsOff();
			_delay_ms(BLINK_DELAY_MS/5);
			
			if(i%period==0) {
				activateLed((int[]){1, 1});
				_delay_ms(BLINK_DELAY_MS/5);
				turnLedsOff();
			}
		}	
}

void activateAllLEds(int numberOfRepetitions) {
	for(int i=0; i<numberOfRepetitions; i++) {
		activateLed((int[]){1, 1});
		_delay_ms(BLINK_DELAY_MS/5);
		turnLedsOff();
		_delay_ms(BLINK_DELAY_MS/5);
	}
}

int main(void)
{
	setup();
	
    while(1) {
		activateOneLedConsequentlyAndAllLEDsPeriodically(1, 0, 15, 3);
		
		activateOneLedConsequentlyAndAllLEDsPeriodically(0, 1, 12, 2);
		
		activateAllLEds(5);
		
	    // turn LED off
	    turnLedsOff();
	    _delay_ms(BLINK_DELAY_MS/5);
    }
}

