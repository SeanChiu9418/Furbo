/*
 * MyMCU_PWM.h
 *
 *  Created on: Oct 8, 2020
 *      Author: SeanChiu
 */

#ifndef CORE_INC_MYMCU_PWM_H_
#define CORE_INC_MYMCU_PWM_H_

#include "MyMCU_Inc.h"

typedef enum
{
	PWM_0 = 0,
	PWM_1,
	PMM_Num
}PWM_Index;

void MyPWM_Init();
void MyPWM_Config(PWM_Index CH, uint32_t Freq, uint16_t Duty);

#ifdef MyFunc_CLI

void MyPWM_CLI_Config(void);
void MyPWM_CLI_Enable(void);
void MyPWM_CLI_DisEnable(void);
void MyPWM_CLI_Start(void);
void MyPWM_CLI_Stop(void);

#define PWM_CLI_TABLE \
{ "~~~~~ PWM_TABLE Start ~~~~~", MyPWM_Init, "" , "" },\
{ "PWM_Init",	MyPWM_Init, "", " PWM INIT" },\
{ "PWM_Conf",	MyPWM_CLI_Config, "uwu",	" Config PWM Channel Freq Duty\r\n PWM Channel 0:PWM0 ~ 1:PMW1 \r\n PWM Freg: 0~24000000 Hz \r\n PWM Duty 0~100 % \r\n Ex: PWM_Conf 0 20000 50" },\
{ "PWM_En",		MyPWM_CLI_Enable, "u",	" Enable PWM Channel\r\n PWM Channel 0:PWM0 ~ 1:PMW1 \r\n Ex: PWM_En 0"},\
{ "PWM_DEn",	MyPWM_CLI_DisEnable, "u", " DisEnable PWM Channel\r\n PWM Channel 0:PWM0 ~ 1:PMW1 \r\n Ex: PWM_DisEn 0"},\
{ "PWM_Start",	MyPWM_CLI_Start, "u", " PWM Channel Start\r\n PWM Channel 0:PWM0 ~ 1:PMW1 \r\n Ex: PWM_Start 0"},\
{ "PWM_Stop",	MyPWM_CLI_Stop, "", " PWM Channel Stop\r\n PWM Channel 0:PWM0 ~ 1:PMW1 \r\n Ex: PWM_Stop 0"},\
{ "~~~~~ PWM_CLI_TABLE End ~~~~~", MyPWM_Init, "", "" }


void MyGPIO_Init();
void MyGPIO_CLI_WritePin();
void MyGPIO_CLI_ReadPin();
void MyGPIO_CLI_Set2InputMode();
void MyGPIO_CLI_Set2OutputMode();
void MyGPIO_IRQHandler();
#endif

#endif /* CORE_INC_MYMCU_PWM_H_ */
