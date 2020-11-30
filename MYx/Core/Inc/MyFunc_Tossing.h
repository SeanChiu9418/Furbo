/*
 * MyFunc_Tossing.h
 *
 *  Created on: Sep 28, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MYFUNC_TOSSING_H_
#define INC_MYFUNC_TOSSING_H_

#include "MyDev_Inc.h"

typedef enum 
{
	Toss_UnInit = 0,
	Init_Ready,
	BlankingHome_Ready,
	Start_Throwing,		
	Start_Blanking,		
	Finish_Blanking,	//5
	Finish_Throwing,
}Toss_Process_Status;

typedef enum
{
	Toss_NoError = 0,
	Toss_Init_Error,
	Toss_BlankingHome_Error,
	Toss_Throwing_Error,
	Toss_Blanking_Error,
	Toss_Process_Error,
	Toss_ThrowJam_Error,
}Toss_Error_Code;

void MyTossing_Init(void);
void MyTossing_BlankingHome(void);
int MyTossing_GetErrorCode();
int MyTossing_GetStatus();
void MyTossing_StartThrowing();
void MyTossing_PTZHome();

#ifdef MyFunc_CLI

//void MyMoto_CLI_STPStop(void);
//void MyMoto_CLI_STPStart(void);
//void MyMoto_CLI_STPMTSet(void);
//void MyMoto_CLI_STPMTStart(void);
void MyTossing_CLI_GetErrorCode(void);
void MyTossing_CLI_GetStatus(void);

#define Tossing_CLI_TABLE \
{ "~~~~~ Tossing_CLI_TABLE Start ~~~~~", MyTossing_Init, "" , "" },  \
{ "TS_I",		MyTossing_Init, "" , "	: Tossing, Motor, MTENC, PWM init" },  \
{ "TS_PH",		MyTossing_PTZHome, "" , "	: Tossing, PTZ STP Motor Back Home" },  \
{ "TS_BH",		MyTossing_BlankingHome,	""	,	"	: Blanding(STPMT_1) back to Home Position."},\
{ "TS_ST",		MyTossing_StartThrowing,	""	,	"	: Start Throwing."},\
{ "TS_GS",		MyTossing_CLI_GetStatus,	""	,	"	: Get Tossing Status\r\n	0: uninit, 1:Init_Ready, 2:BlankingHome_Ready, 3:Start_Throwing, 4:Start_Blanking, 5:Finish_Blanking, 6:Finish_Throwing"},\
{ "TS_GE",		MyTossing_CLI_GetErrorCode,	""	,	"	: Get Tossing ErrorCode\r\n	0: Toss_NoError, 1:Toss_Init_Error, 2:Toss_BlankingHome_Error, 3:Toss_Throwing_Error, 4:Toss_Blanking_Error"},\
{ "~~~~~ Tossing_CLI_TABLE End ~~~~~", MyTossing_Init, "", "" }

//{ "MT_S",      MyMoto_CLI_STPStart, "vuu", "Motor start & Set Speed, Angle, Direct setting. ex: \"Motor_Start Angle Direct Speed\"\r\n Angle: 0~360 degree \r\n Direct 0:CW 1:CCW \r\n Speed 0:High 1:Low" },\

#endif

#endif /* INC_MYFUNC_TOSSING_H_ */
