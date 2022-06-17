/*
 * Arduino_LCD_DHT11.c
 *
 * Created: 04/06/2022 00:16:14
 * Author : nikita.nikitin
 */ 

#define F_CPU 16000000UL

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <avr/interrupt.h>

#include "bit_definitions.h"
#include "ddr_definitions.h"
#include "port_definitions.h"
#include "lcd_functions.h"
#include "lcd_operations.h"

// utils
#include "char_utils.h"

#define ARR_LEN(arr) ((int) (sizeof (arr) / sizeof(arr)[0]))
#define HIGH_TEMPERATURE 28.0

const uint8_t PROGRAM_AUTHOR[]   = "Nikita Nikitins";
const uint8_t PROGRAM_VERSION[]  = "LCD-AVR-4d (gcc)";
const uint8_t PROGRAM_CREATED_TEXT[] = "Created:";
const uint8_t PROGRAM_DATE[]     = "June 4, 2022";
const uint8_t TEMPERATURE_STRING[]     = "Temperature:";
const uint8_t HIGH_TEMPERATURE_STRING[]     = "HIGH Temperature!";
const uint8_t HUMIDITY_STRING[]     = "Humidity:";

uint8_t c=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum;
float humidityCurr=0.0, temperatureCurr=0.0;

void setup();
void initLCD();
void displayInitialText();

void dhtRequest();
void dhtResponse();
uint8_t dhtReceiveData();
float getMeasurementFromDht(uint8_t intValue, uint8_t decimalValue, char* valueString, char* postFix);
void readDHTSensorData();

void buzzerPlay(float duration, float frequency);
void processCriticalTemperature();

// Timer logic
void setupTimer();
volatile int in_counter = 0;
bool showTemperature = true;

volatile bool executeTimer = false;

int main(void) {
	// Pause interrupts
	cli(); 
	setup(); // also shows initial text
	
	setupTimer();
	
    /* Replace with your application code */
    while(1) {
		if (executeTimer) {
			if (in_counter == 0) {
				lcd_clear_screen();
				if (showTemperature) {
					lcd_write_string_4d(TEMPERATURE_STRING);
					} else {
					lcd_write_string_4d(HUMIDITY_STRING);
				}
			}
			
			in_counter++;
			
			//if(in_counterThread1<= 60)
			
			// Start Thread1
			// Every 30s
			
			if(in_counter>=30) {
				showTemperature = !showTemperature;
				in_counter =0;
			}
			
			readDHTSensorData();
			
			if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum) {
				lcd_clear_screen();
				lcd_write_string_4d("DHT11_READ_ERR");
				} else {
				lcd_write_instruction_4d(LCD_SET_CURSOR | LCD_LINE_TWO);
				
				char humidityStringValue[20];
				humidityCurr = getMeasurementFromDht(I_RH, D_RH, humidityStringValue, "%");
				
				char temperatureStringValue[20];
				temperatureCurr = getMeasurementFromDht(I_Temp, D_Temp, temperatureStringValue, "C");
				
				if (temperatureCurr > HIGH_TEMPERATURE) {
					processCriticalTemperature();
				}
				
				if (showTemperature) {
					lcd_write_string_4d(temperatureStringValue);
					} else {
					lcd_write_string_4d(humidityStringValue);
				}
			}
			
			executeTimer = false;
		}
	}
		
	return 0;
}


void setup() {
	// configure the microprocessor pins for the data lines
	LCD_DATA_BUS_LINE_7_DDR |= 1<<LCD_DATA_BUS_LINE_7_BIT;
	LCD_DATA_BUS_LINE_6_DDR |= 1<<LCD_DATA_BUS_LINE_6_BIT;
	LCD_DATA_BUS_LINE_5_DDR |= 1<<LCD_DATA_BUS_LINE_5_BIT;
	LCD_DATA_BUS_LINE_4_DDR |= 1<<LCD_DATA_BUS_LINE_4_BIT;
	LCD_ENABLE_MODULE_DDR |= 1<<LCD_ENABLE_MODULE_BIT;
	LCD_REGISTER_SELECT_DDR |= 1<<LCD_REGISTER_SELECT_BIT;
	
	initLCD();
}

void initLCD(){
	lcd_init_4d(); // initialize the LCD display for a 4-bit interface
	
	displayInitialText();
}

void displayInitialText() {
	uint8_t* initialTextCollections[2][2] = {{PROGRAM_AUTHOR, PROGRAM_VERSION}, {PROGRAM_CREATED_TEXT, PROGRAM_DATE}};
	
	for(int i=0;i<ARR_LEN(initialTextCollections); i++) {
		displayTextTwoLines(initialTextCollections[i][0], initialTextCollections[i][1]);
		_delay_ms(2000);
		lcd_clear_screen();
	}
}

void dhtRequest() {
	DHT11_OUTPUT_DDR |= (1<<DHT11_OUTPUT_BIT);		// pinMode(dht11_pin, OUTPUT);
	DHT11_OUTPUT_PORT &= ~(1<<DHT11_OUTPUT_BIT);	// digitalWrite(dht11_pin, LOW); 
	_delay_ms(20);
	DHT11_OUTPUT_PORT |= 1<<DHT11_OUTPUT_BIT;		//digitalWrite(dht11_pin, HIGH);
}

void dhtResponse() {
	// PORTx is for sending a value out to the port when it is set as an output. PINx is for use in getting the current value from the port when it is set as an input.
	// PORTx also sets the state of the internal pull-up resistors when the port is set to input. And on more recent models, writing to PINx will toggle the value of the pins (the PORTx value) when they are set as outputs.
	
	DHT11_OUTPUT_DDR &= ~(1<<DHT11_OUTPUT_BIT);
	while(PIND & (1<<DHT11_OUTPUT_BIT));
	while((PIND & (1<<DHT11_OUTPUT_BIT))==0);
	while(PIND & (1<<DHT11_OUTPUT_BIT));
}

uint8_t dhtReceiveData() {
	int bitCount = 8;
	
	for(int q=0; q<bitCount; q++) {
		while((PIND & (1<<DHT11_OUTPUT_BIT)) == 0);  /* check received bit 0 or 1 */
		_delay_us(30);
		if(PIND & (1<<DHT11_OUTPUT_BIT))/* if high pulse is greater than 30ms */
			c = (c<<1)|(0x01);	/* then its logic HIGH */
		else			/* otherwise its logic LOW */
			c = (c<<1);
		while(PIND & (1<<DHT11_OUTPUT_BIT));
	}
	
	return c;
}

float getMeasurementFromDht(uint8_t intValue, uint8_t decimalValue, char* valueString, char* postFix) {
	char data[5];
	itoa(intValue,data,10); // byte or int to char array
	float measurement = atoi(data);
	
	itoa(decimalValue,data,10);
	measurement += (atoi(data) / 10);
	
	ftoa(measurement, valueString, 2);
	strncat(valueString, postFix, 1);
	
	return measurement;
}

void buzzerPlay(float duration, float frequency) {
	// Physics
	long int i, cycles;
	float wavelength;
	
	wavelength = (1/frequency)*1000;
	cycles=duration/wavelength;
	
	float half_period = wavelength/2;
	
	BUZZER_DDR |= (1<<BUZZER_BIT);	
	
	for (i=0;i<cycles;i++) {
		_delay_ms(half_period);
		PORTD |= (1 << BUZZER_BIT);
		_delay_ms(half_period);
		PORTD &= ~(1 << BUZZER_BIT);
	}
}

void readDHTSensorData() {
	dhtRequest();
	dhtResponse();
	I_RH=dhtReceiveData();	/* store first eight bit in I_RH */
	D_RH=dhtReceiveData();	/* store next eight bit in D_RH */
	I_Temp=dhtReceiveData();	/* store next eight bit in I_Temp */
	D_Temp=dhtReceiveData();	/* store next eight bit in D_Temp */
	CheckSum=dhtReceiveData();/* store next eight bit in CheckSum */
}

void processCriticalTemperature() {
	buzzerPlay(400,1600);
	buzzerPlay(400,1000);
}

// Timer
// A timer is basically nothing else than a certain register in the micro, which is increased (or decreased) continuously by 1 under hardware control.
// Instead of coding instructions in the program that are executed regularly and increment a register by 1, the microcontroller does this all by itself!

// ATMEGA328P: 3 timers
// 8 Bit-Timer0: used for functions millis(), micros(), delay() and for PWM at pin D5 and D6
// 16 Bit Timer1: Use e.g. for the Servo, VirtualWire and TimerOne library and for PWM at pin D9 and D10
// 8 Bit Timer2: Used for function tone() and for PWM at pin D3 and D11
// The system clock of the Arduino Uno is 16 MHz (CPU frequency). This means Timer0, Timer1 and Timer2 increase 16 million times per second
//  For example, the 8 bit timers count from 0 to 255 each time. At 256 an overflow occurs and the timers start again from 0. This means 16000000/256 = 62500 overflows per second (62.5kHz clock rate). This is likely too fast for most timer applications!
// prescalers - allows you to divide the system clock (16MHz) by the selected factor( 1, 8, 64, 256 or 1024) and set a lower clock rate for the timers 
//  For example, a prescaler of 1024 would increase the timer registers by 1 only at the 1024th system clock pulse. This would be 16000000/1024=15625 increments per second and thus with an 8 bit timer 15625/256= 61.035 overflows per second (~61 Hz clock rate of the timer).

// CTC mode ("Clear Timer on Compare Mode"). The counter is cleared when the value of the counter (TNCT1) matches either the value of the OCR1A register or the value of the ICR1 register (in our case OCR1A). So the OCR1A register determines the maximum value of the counter and thus its resolution.
// 16 Bit Timer1 needes the following registers:
// Timer Count Register 1: TCNT1
// Output Compare Register A: OCR1A
// Timer Counter Control Register A: TCCR1A
//									 TCCR1B
// Timer/Counter Interrupt Mask Regster: TIMSK1
// OCR1A = (CPU Frequency/ (Prescaler*Interrupt Frequency)) - 1   
/* CPU F (A UNO) = 16*10^6
* 50 Hz = (20ms period duration)
* OCR1A= (16.000.000 / (1024 * 50)) – 1 = 311,5  -> prescaler 1024
* OCR1A= (16,000,000 / (8 * 50)) – 1 = 39,999 -> prescaler 8
* ATTENTION: The OCR1A value must be less than 65.536 (2^16 )!
* Count = (Required Delay * Clock Frequency)/Prescaler – 1
* Therefore a prescaler 8 can NOT reach 10Hz interrupt frequency: OCR1A= (16.000.000 / (8 * 10)) – 1 = 199.999
* https://nerd-corner.com/arduino-timer-interrupts-how-to-program-arduino-registers/
* https://github.com/TheMACLayer/atmega328p-tutorial/blob/master/Ep3_register_logic/register%20_logic.md
*/
void setupTimer() {
	 TCCR1A = 0;
	 TCCR1B = 0;
	 TCNT1  = 0;
	 
	 OCR1A = 15624; // 1s; One tick is used to return to zero // (30 * 16000000)/256 - 1 = 1874999 the number is bigger, than is allowed, not more than 4.19 second with oscilator = 1024; I used 1s
	
	// Clear timer on compare match
	// The timer resets itself when it reaches 15625 (OCR1A +1)
	TCCR1B |= (1 << WGM12);
	
	// Set the prescaler to 1024 (See ATMega328PU datasheet for infos)
	TCCR1B |= (1 << CS12) | (1 << CS10);
	
	// Enable timer interrupt
	TIMSK1 |= (1 << OCIE1A);

	// Enable interrupts
	sei();             // 16 bit register
}

// ISR
ISR(TIMER1_COMPA_vect) {
	
	executeTimer = true;
}

