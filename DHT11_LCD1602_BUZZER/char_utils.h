/*
 * char_utils.h
 *
 * Created: 04/06/2022 23:59:13
 *  Author: nikita.nikitin
 */ 

#include <string.h>
#include <math.h>

void reverse(char* str, int len);
int intToStr(int x, char str[], int d);
void ftoa(float n, char* res, int afterpoint);