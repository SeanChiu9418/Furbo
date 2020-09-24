/*
 * My_Config.h
 *
 *  Created on: Jul 6, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MY_CONFIG_H_
#define INC_MY_CONFIG_H_

#define FW_VERSION      "0.0.2 "

/* ########################## Project Selection ############################## */
#define Samplisafe
#define Tomofun

/* ########################## MCU Selection ############################## */
//#define MCU_STM32L053
//#define MCU_STM32G071
#define MCU_M252SD

#if defined(MCU_STM32L053) | defined(MCU_STM32G071)
#define MCU_STM32
#endif

#if defined(MCU_M252SD) | defined(MCU_M251LE)
#define MCU_Nuvoton
#endif


/* ########################## MCU Function Selection ############################## */
#define MCU_GPIO
#define MCU_SYS	// MCU Internal SYS Function like Delay, Clock...
#define MCU_UART
//#define MCU_SPI

/* ########################## Device Selection ############################## */
#define Dev_PIR_1598
//#define Dev_SubG_S2LP
//#define Dev_LED
//#define Dev_Motor

/* ########################## My Function Selection ############################## */
#define MyFunc_CLI

/* ########################## Selected Item Enable ############################## */
// Include MCU .h File //
#ifdef MCU_STM32
	#ifdef MCU_STM32L053
		#include "stm32l0xx_hal.h"
	#elif defined(MCU_STM32G071)
		#include "stm32g0xx_hal.h"
	#endif
	#include "main.h"
#endif

#ifdef MCU_Nuvoton
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

#ifdef MCU_SPI
#include "MyMCU_SPI.h"
#endif

// Include Selected Item
#ifdef Dev_PIR_1598
 #include "MyDev_PIR.h"
#endif

#ifdef Dev_SubG_S2LP
 #include "MyDev_SubG.h"
#endif

#ifdef Dev_LED
 #include "MyDev_LED.h"
#endif

#ifdef Dev_Motor
 #include "MyDev_Motor.h"
#endif

#ifdef MyFunc_CLI
 #include "MyFunc_CLI.h"
#endif

#endif /* INC_MY_CONFIG_H_ */
