/*
 * My_Func.c
 *
 *  Created on: Jul 6, 2020
 *      Author: SeanChiu
 */

#include <string.h>
#include <stdio.h>
#include "MyMCU_Inc.h"

char * int2str(int value)
{
	unsigned int Data = (unsigned int)value;
	static char str[10];
  	sprintf(str, "%d", Data);
  	return str;
}

uint8_t My_CharDowncase(uint8_t c)
{
  if ('A' <= c && c <= 'Z')
    return c + 'a' - 'A';
  else
    return c;
}

// Returns a value > 15 if ch is not a hex digit.
uint8_t My_HexToInt(uint8_t ch)
{
  return ch - (ch >= 'a' ? 'a' - 10
               : (ch >= 'A' ? 'A' - 10
                  : (ch <= '9' ? '0'
                     : 0)));
}
