/*
 * MyDev_MotorEncoder.h
 *
 *  Created on: Oct 5, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MYDEV_MOTORENCODER_H_
#define INC_MYDEV_MOTORENCODER_H_

#include "MyMCU_Inc.h"

typedef enum
{
	ENC_STPMT1 = 0,
	ENC_DCMT,
	ENC_NUM
}MTENC_Index;

extern Timer_Index MTENC[ENC_NUM];

//#define STPMT1_Timer Timer_0
//#define DCMT_Timer Timer_1

void MyMTENC_Init();

#endif /* INC_MYDEV_MOTORENCODER_H_ */
