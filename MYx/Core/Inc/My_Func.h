/*
 * My_Func.h
 *
 *  Created on: Jul 6, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MY_FUNC_H_
#define INC_MY_FUNC_H_


#include "My_Config.h"

#define true 1
#define false 0
//#define TRUE 1
//#define FALSE 0

#define S_ABS(a) ((a)>0?(a):-(a))

char * int2str(int value);
uint8_t My_CharDowncase(uint8_t c);
uint8_t My_HexToInt(uint8_t ch);
#endif /* INC_MY_FUNC_H_ */
