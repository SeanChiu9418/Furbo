/*
 * MyMCU_SYS.h
 *
 *  Created on: Jul 8, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MYMCU_SYS_H_
#define INC_MYMCU_SYS_H_

#include "My_Config.h"


#ifdef MCU_STM32

#define NVIC_GPIO0_1 EXTI0_1_IRQn
#define NVIC_GPIO2_3 EXTI2_3_IRQn
#define NVIC_GPIO4_15 EXTI4_15_IRQn

#define MyNVIC_SetPriority(A,B,C) 	HAL_NVIC_SetPriority(A, B, C)
#define MyNVIC_EnableIRQ(A) 		HAL_NVIC_EnableIRQ(A)
#define MyNVIC_DisableIRQ(A)		HAL_NVIC_DisableIRQ(A)
#define My_Delay(A) 				HAL_Delay(A)
#define My_DelayUs(A) 				My_usDelay(A)

#elif defined(MCU_Nuvoton)

#define My_Delay(A) 				CLK_SysTickLongDelay(A*1000)
#define My_DelayUs(A) 				CLK_SysTickDelay(A)

#endif

void MySYS_GetSYSClock();
void My_usDelay(int us);
#endif /* INC_MYMCU_SYS_H_ */
