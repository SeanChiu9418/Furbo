/*
 * MyDev_MotorEncoder.h
 *
 *  Created on: Oct 5, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MYDEV_MOTORENCODER_H_
#define INC_MYDEV_MOTORENCODER_H_

//#include "MyMCU_Inc.h"
#include "MyDev_Inc.h"

#define ENC_GPIO_Blanking	GPIO_INT0
#define ENC_GPIO_Throw		GPIO_INT1
#define ENC_GPIO_PTZ		GPIO_INT2

typedef enum
{
	ENC_Blanking = 0,
	ENC_Throw,
	ENC_PTZ,
	ENC_NUM
}MTENC_Index;

typedef enum
{
	ENC_Start = 0,
	ENC_TimeOut,
	ENC_Stabled,
	ENC_UnStable,
	ENC_GetInterrupt,
	ENC_NUM_Status	
}MTENC_Status;

extern Timer_Index MTENC[ENC_NUM];

//#define STPMT1_Timer Timer_0
//#define DCMT_Timer Timer_1

void MyMTENC_Init();
void MyMTENC_NVIC_STPMT1();
void MyMTENC_NVIC_DCMT(uint32_t timer_cnt);
void MyMTENC_CheckHome();
void MyMTENC_InHome(MTENC_Index index, int IsHome);
int MyMTENC_IsInHome(MTENC_Index index);
int MyMTENC_GetDCStatus();
char MyMTENC_IsIntDetect();
void MyMTENC_ClearIntDetect();
void MyMTENC_SetDCStatus(MTENC_Status status);
#endif /* INC_MYDEV_MOTORENCODER_H_ */
