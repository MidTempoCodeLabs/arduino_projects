/*
 * display_digit_helper.h
 *
 * Created: 5/11/2022 12:58:51 AM
 *  Author: nikita.nikitin
 */ 


#ifndef DISPLAY_DIGIT_HELPER_H_
#define DISPLAY_DIGIT_HELPER_H_

#include "segment_relateive_pin_defitions.h"


extern const uint8_t digit_pins_lookup[10][8];

void displayDigit(int digit);
void turnOff();


#endif /* DISPLAY_DIGIT_HELPER_H_ */