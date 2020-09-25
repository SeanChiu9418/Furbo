/*
 * MyMCU_I2C.h
 *
 *  Created on: Sep 18, 2020
 *      Author: SeanChiu
 */

#ifndef CORE_INC_MYMCU_I2C_H_
#define CORE_INC_MYMCU_I2C_H_

#include "My_Config.h"

typedef enum
{
	A1D1 = 0,	//Address 1 Byte, Data 1 Byte
	A1D2,		//Address 1 Byte, Data 2 Byte
	A2D1,		//Address 2 Byte, Data 1 Byte
	A2D2		//Address 2 Byte, Data 2 Byte
}I2CType;

void MyI2C_Init();
void MyI2C_SelfTest();
void MyI2C_Test();
#endif /* CORE_INC_MYMCU_I2C_H_ */
