/*
 * pin_connection_constants.h
 *
 * Created: 04/06/2022 00:36:21
 *  Author: nikita.nikitin
 */ 

/* 
VSS - GROUND Potential
VDD 5v - Positive Voltage
V0 - Contract Adjustment
RS - Register Select
RW - READ wRITE sELECT Pin
E - A Enable Pint to Enable LCD mODULE
D0-D7 LCD Data Bus Line
LED+ Back Light Source Led Anode
LCD- - Back Light Soyrce LED Cathode

RW is grounded
VSS is grounded
V0 is connected to potentiometer
VDD is connected to pos line on board
board is connected to 5V on microcontroller
*/

#define LCD_DATA_BUS_LINE_7_PIN_NUMBER 12
#define LCD_DATA_BUS_LINE_6_PIN_NUMBER 11
#define LCD_DATA_BUS_LINE_5_PIN_NUMBER 10
#define LCD_DATA_BUS_LINE_4_PIN_NUMBER 9
#define LCD_ENABLE_MODULE_PIN_NUMBER 8
#define LCD_REGISTER_SELECT_PIN_NUMBER 7
#define BTN_INPUT_PIN_NUMBER 4
#define BUZZER_PIN_NUMBER 3
#define DHT11_OUTPUT_PIN_NUMBER 2