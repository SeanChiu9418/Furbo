/*
 * My_Config.h
 *
 *  Created on: Jul 6, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MY_CONFIG_H_
#define INC_MY_CONFIG_H_

#define FW_VERSION      "0.1.0 "

/* ########################## Project Selection ############################## */
#define Samplisafe
#define Tomofun

/* ########################## MCU Selection ############################## */
//#define MCU_STM32L053
//#define MCU_STM32G071
#define MCU_M252SD

/* ########################## MCU Function Selection ############################## */
#define MCU_GPIO
#define MCU_SYS	// MCU Internal SYS Function like Delay, Clock...
#define MCU_UART
#define MCU_I2C
//#define MCU_SPI
#define MCU_Timer
#define MCU_PWM

/* ########################## Device Selection ############################## */
//#define Dev_PIR_1598
//#define Dev_SubG_S2LP
//#define Dev_LED
//#define Dev_LED_KTD2027
#define Dev_Motor
#define Dev_MotorEncoder
//#define Dev_MIC_VM3011

/* ########################## My Function Selection ############################## */
#define MyFunc_SelfTest
#define MyFunc_CLI
#define MyFunc_Tossing


/* ########################## Selected Item Enable ############################## */

#if defined(MCU_STM32L053) | defined(MCU_STM32G071)
#define MCU_STM32
#endif

#if defined(MCU_M252SD) | defined(MCU_M251LE)
#define MCU_Nuvoton
#endif




#endif /* INC_MY_CONFIG_H_ */
