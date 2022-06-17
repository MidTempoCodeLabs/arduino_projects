/*
 * arduino_BIT_definitions.h
 *
 * Created: 04/06/2022 00:35:24
 *  Author: nikita.nikitin
 */ 

#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>

#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#include <stdbool.h>

#include "pin_connection_constants.h"

extern const int bit_lookup[14];

#ifdef LCD_DATA_BUS_LINE_7_PIN_NUMBER
#define LCD_DATA_BUS_LINE_7_BIT bit_lookup[LCD_DATA_BUS_LINE_7_PIN_NUMBER]
#endif

#ifdef LCD_DATA_BUS_LINE_6_PIN_NUMBER
#define LCD_DATA_BUS_LINE_6_BIT bit_lookup[LCD_DATA_BUS_LINE_6_PIN_NUMBER]
#endif

#ifdef LCD_DATA_BUS_LINE_5_PIN_NUMBER
#define LCD_DATA_BUS_LINE_5_BIT bit_lookup[LCD_DATA_BUS_LINE_5_PIN_NUMBER]
#endif

#ifdef LCD_DATA_BUS_LINE_4_PIN_NUMBER
#define LCD_DATA_BUS_LINE_4_BIT bit_lookup[LCD_DATA_BUS_LINE_4_PIN_NUMBER]
#endif

#ifdef LCD_ENABLE_MODULE_PIN_NUMBER
#define LCD_ENABLE_MODULE_BIT bit_lookup[LCD_ENABLE_MODULE_PIN_NUMBER]
#endif

#ifdef LCD_REGISTER_SELECT_PIN_NUMBER
#define LCD_REGISTER_SELECT_BIT bit_lookup[LCD_REGISTER_SELECT_PIN_NUMBER]
#endif

#ifdef BTN_INPUT_PIN_NUMBER
#define BTN_INPUT_BIT bit_lookup[BTN_INPUT_PIN_NUMBER]
#endif

#ifdef DHT11_OUTPUT_PIN_NUMBER
#define DHT11_OUTPUT_BIT bit_lookup[DHT11_OUTPUT_PIN_NUMBER]
#endif

#ifdef BUZZER_PIN_NUMBER
#define BUZZER_BIT bit_lookup[BUZZER_PIN_NUMBER]
#endif