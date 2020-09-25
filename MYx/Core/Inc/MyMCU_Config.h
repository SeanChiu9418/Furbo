/*
 * MyMCU_Config.h
 *
 *  Created on: Sep 17, 2020
 *      Author: SeanChiu
 */

#ifndef CORE_INC_MYMCU_CONFIG_H_
#define CORE_INC_MYMCU_CONFIG_H_

#include "My_Config.h"

#ifdef MCU_STM32
#ifdef MCU_UART
#include "stm32g0xx_hal_uart.h"
#endif
#endif
#endif /* CORE_INC_MYMCU_CONFIG_H_ */
