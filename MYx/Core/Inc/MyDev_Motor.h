/*
 * MyDev_Motor.h
 *
 *  Created on: Aug 12, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MYDEV_MOTOR_H_
#define INC_MYDEV_MOTOR_H_

#include "MyMCU_Inc.h"

typedef enum
{
	CLI_Motor_Init = 0,
	CLI_Motor_SetSpeed,
	CLI_Motor_SetAngle,
	CLI_Motor_SetDirect,
	CLI_Motor_Start,
	CLI_Motor_StartEx, // Without re-set Speed, Angle, Direct
	CLI_Motor_Func_Num
}Motor_CLI_Func;

typedef enum
{
	SS_High = 1,
	SS_Low,
	SS_Num
}STPMT_Speed;

typedef enum
{
	CW = 0, //clockwise = 0,
	CCW, 	//anti-clockwise
	MotorDirect_Num
}MotorDirect;

typedef enum
{
	STPMT_1 = 0,
	STPMT_2,
	STPMT_Num
}STPMT_No;

typedef enum
{
	STPMT_Phase0 = 0,
	STPMT_Phase1,
	STPMT_Phase2,
	STPMT_Phase3,
	STPMT_Phase_Num
}STPMT_Phase;

typedef struct
{
	GPIO_Port_Index Port;
	GPIO_Pin_Index Pin;
}STPMT_GPIO;

typedef struct
{
	STPMT_GPIO IN1;	
	STPMT_GPIO IN2;	
	STPMT_GPIO IN3;	
	STPMT_GPIO IN4;	
	STPMT_Speed Speed;
	MotorDirect	Direct;
	int Angle;
} STPMT_Handle;

void MyMoto_Init();
void MyMoto_SetSTP_Ang(int Ang);
void MyMoto_SetSTPSpeed(STPMT_Speed Speed);
void MyMoto_STPStart(int angle, MotorDirect direction, STPMT_Speed Speed);
void MyMoto_STPStartEx(void);
void MyMoto_STPStartCCEx(void);
void MyMoto_CLI_STPStart(void);
void MyMoto_STPStartDemo(void);
void MyMoto_CLI_STPMTSet(void);
void MyMoto_CLI_STPMTStart(void);

#ifdef MyFunc_CLI
#define Motor_CLI_TABLE \
{ "~~~~~ Motor_CLI_TABLE Start ~~~~~", MyMoto_Init, "" , "" },  \
{ "MT_I",      MyMoto_Init, "" , "	: Motor GPIO/Angle(360d)/Speed(High)/Direct(CW) init" },  \
{ "MT",      MyMoto_CLI_STPMTStart,	"u" , "	: Motor (Number) start. \r\n	MT Number \r\n	Number 0:Motor_1 1:Motor_2 \r\n	ex:\"MT 0" },\
{ "MTC",      MyMoto_STPStartCCEx,	"" , "	: Motor start CCW With existed Angle, Direct, Speed setting" },\
{ "MT_S",      MyMoto_CLI_STPMTSet, "uvuu", "	: Set Stepper Motor (Number), (Angle), (Direction), (Speed). \r\n	MT_S Number Angle Direction Speed \r\n	Number 0:Motor_1 1:Motor_2 \r\n	Angle: 0~360 degree \r\n	Direction 0:CW 1:CCW \r\n	Speed 0:High 1:Low \r\n	ex: \"MT_S 0 360 0 0\" " },\
{ "MT_D",      MyMoto_STPStartDemo,	"u" , "	: Motor Demo (N) times start \r\n	MT_D Times \r\n	ex: \"MT_D 5\""},\
{ "~~~~~ Motor_CLI_TABLE End ~~~~~", MyMoto_Init, "", "" }

//{ "MT_S",      MyMoto_CLI_STPStart, "vuu", "Motor start & Set Speed, Angle, Direct setting. ex: \"Motor_Start Angle Direct Speed\"\r\n Angle: 0~360 degree \r\n Direct 0:CW 1:CCW \r\n Speed 0:High 1:Low" },\

#endif
#endif /* INC_MYDEV_MOTOR_H_ */
