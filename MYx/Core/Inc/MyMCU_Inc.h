/*
 * MyMCU_Inc.h
 *
 *  Created on: Sep 25, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MYMCU_INC_H_
#define INC_MYMCU_INC_H_

#include "My_Config.h"

// Include MCU .h File //
#ifdef MCU_STM32
#include "main.h"
#include "MyMCU_Config.h"
#elif defined(MCU_Nuvoton)
#include "NuMicro.h"
#endif

// Include "General Function" & "MiddleWare" for Driver & Feature Function using"
#include "My_Func.h"

#ifdef MCU_GPIO
#include "MyMCU_GPIO.h"
#endif

#ifdef MCU_UART
#include "MyMCU_UART.h"
#endif

#ifdef MCU_SYS
#include "MyMCU_SYS.h"
#endif

#ifdef MCU_I2C
#include "MyMCU_I2C.h"
#endif

#ifdef MCU_SPI
#include "MyMCU_SPI.h"
#endif


#endif /* INC_MYMCU_INC_H_ */
