/*
 * MyMCU_Timer.h
 *
 *  Created on: Sep 30, 2020
 *      Author: SeanChiu
 */

#ifndef CORE_INC_MYMCU_TIMER_H_
#define CORE_INC_MYMCU_TIMER_H_

#include "MyMCU_Inc.h"



typedef enum
{
	Timer_0 = 0,
	Timer_1,
	Timer_2,
	Timer_3,
	Timer_Num
}Timer_Index;

typedef enum
{
	Counter_Mode = 0,	// TIMER_PERIODIC_MODE for Nuvoton
	PWM_Mode,
	Timer_Mode_Num
	
}Timer_Mode;


extern TIMER_T *Timer_Port[Timer_Num];
extern uint32_t Timer_Module[Timer_Num];


// Timer Start/Stop 
#ifdef MCU_STM32

#define MyTimer_Start(A) HAL_TIM_Base_Start(Timer_Port[(A)])	// ENABLE Timer
	
#elif defined(MCU_Nuvoton)

#define MyTimer_Start(A) TIMER_Start(Timer_Port[(A)])	// ENABLE Timer

#endif
/* ########################## Timer Function Define List ############################## */

#define STPMT1_Timer Timer_0
#define DCMT_Timer Timer_1

#ifdef MyFunc_SelfTest
void MyTimer_TestInit(Timer_Index Index);
#endif
	

#endif /* CORE_INC_MYMCU_TIMER_H_ */
