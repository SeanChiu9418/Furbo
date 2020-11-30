/*
 * MyFunc_Tossing.c
 *
 *  Created on: Sep 28, 2020
 *      Author: SeanChiu
 */

#include <stdio.h>
#include "MyFunc_Tossing.h"

#define Blanking_MaxTimes 2

static Toss_Process_Status Tossing_Status;
static Toss_Error_Code Tossing_Error;

void MyTossing_ThrowingMotorStart(int OnOff)
{
	if(OnOff == true)
	{		
		MyMTENC_Start(ENC_Throw);
		MyMoto_DC_Forward();
	}
	else
	{
		MyMTENC_Stop(ENC_Throw);
		MyMoto_DC_Brake();
	}
}

void MyTossing_StartBlanking()
{
	printf("~~~~~ MyTossing_StartBlanking ~~~~~ \r\n");
	//if((Tossing_Status < Start_Throwing)||(Tossing_Error !=Toss_NoError))
	//{
		//printf("!!!!! Toss_Process_Error !!!!! \r\n");		
		//Tossing_Error = Toss_Process_Error;
		//return;
	//}
	int x,y,total_time;
	Tossing_Status = Start_Blanking;
	MotorDirect dir = CW;
	total_time =0;
	while(1)
	{	
		if(total_time > Blanking_MaxTimes)	// Blanking times over spec.
		{
			//if(x == ENC_GetInterrupt)
			if(MyMTENC_IsIntDetect())
			{
				Tossing_Error = Toss_Blanking_Error;	// jam in blanking shell
				printf("!!!!! Toss_Blanking_Error  total_time =%d !!!!! \r\n",total_time);
			}
			else
			{
				printf("!!!!! Toss_Throwing_Error  total_time =%d !!!!! \r\n",total_time);
				Tossing_Error = Toss_Throwing_Error;	// Dog food is empty
			}
			break;
		}
		
		total_time ++;
		MyMTENC_Start(ENC_Blanking);
		MyMoto_STPSet(STPMT_1, 360, dir, SS_High);
		MyMoto_STPStartX(STPMT_1);
		
		//x = MyMTENC_IsIntDetect();
		MyMTENC_CheckHome();
		
		//if(x == ENC_GetInterrupt)	// Check if dog food are throwed
		if(MyMTENC_IsIntDetect())	// Check if dog food are throwed
		{

			if(MyMTENC_IsInHome(ENC_Blanking)) // Check if Blanking is back to home
			{
				Tossing_Status = Finish_Blanking;	// throwed & back to home
				break;
			}
			else
			{
				printf("!!!!! Toss_Blanking_Error Motor Start CCW !!!!! \r\n");
				if (total_time % 2)
					dir = CCW;	
				else
					dir = CW;
				continue;
			}				
		}
		else
		{

			if(MyMTENC_IsInHome(ENC_Blanking))
			{
				printf("!!!!! Toss_Throwing_Error Food Empty !!!!! \r\n");
				continue;
			}
			else
			{
				printf("!!!!! Toss_Throwing_Error Food Empty & Toss_Blanking_Error Motor Start CCW!!!!! \r\n");
				if (total_time % 2)
					dir = CCW;	
				else
					dir = CW;				
				continue;
			}
		}
	}
		
	MyMTENC_Stop(ENC_Blanking);
}

void MyTossing_StartThrowing()
{	
	int x;
	static int JamCnt = 0;
	MyMTENC_CheckHome();
	/*
	if((Tossing_Status < BlankingHome_Ready)||(!MyMTENC_IsInHome(ENC_Blanking)))
	{
		printf("!!!!! Blanking Not Back to Home Yet or Blank Home Position Error, Check it !!!!!\r\n");
		return;
	}
	if(Tossing_Error != Toss_NoError)
	{
		printf("!!!!!  Error:%d , Check it !!!!!\r\n",Tossing_Error);
		return;
	}
		*/
	MyTossing_ThrowingMotorStart(true);
	Tossing_Status = Start_Throwing;
	MyMTENC_SetDCStatus(ENC_Start);
	MyMTENC_SetInterruptCnt(0);
	MyMTENC_ClearIntDetect();
	JamCnt = 0;
	while(1)
	{	
		x = MyMTENC_GetDCStatus();
		if( x == ENC_UnStable)
		{
			//MyMoto_DC_Brake();
			MyTossing_ThrowingMotorStart(false);
			Tossing_Error = Toss_Throwing_Error;
			return;
		}
		else if(x == ENC_Stabled)
			break;
		else if(x == ENC_TimeOut)
		{
			JamCnt ++;
			if (JamCnt >=2)
			{				
				Tossing_Error = Toss_ThrowJam_Error;
				printf("!!!!!Tossing_Error ThrowJam JamCnt=%d !!!!!\r\n",JamCnt);
				MyTossing_ThrowingMotorStart(false);
				return;
			}
			
			MyMoto_DC_Brake();
			My_Delay(100);
			MyMoto_DC_Reverse();		
			My_Delay(1000);
			MyMoto_DC_Brake();
			My_Delay(100);
			MyMoto_DC_Forward();		
			My_Delay(1000);			
			MyMTENC_SetDCStatus(ENC_Start);
			My_Delay(100);			
		}
	}		
	
	MyTossing_StartBlanking();	
	x = MyMTENC_GetDCStatus();	
	MyTossing_ThrowingMotorStart(false);
	
	if(x == ENC_GetInterrupt)
	{
		My_Delay(100);
		MyMoto_DC_Reverse();		
		My_Delay(2000);
		MyMoto_DC_Brake();
		My_Delay(100);
		MyMoto_DC_Forward();		
		My_Delay(2000);
		MyMoto_DC_Brake();
		Tossing_Error = Toss_ThrowJam_Error;
	}
		
}

int MyTossing_GetErrorCode()
{ 
	printf("MyTossing_GetErrorCode=%d \r\n", Tossing_Error);
	return Tossing_Error;
}

int MyTossing_GetStatus()
{ 
	printf("MyTossing_GetStatus=%d \r\n", Tossing_Status);
	return Tossing_Status;
}

void MyTossing_PTZHome()
{
	MyMoto_STPSet(STPMT_2, 270, CW, SS_High);
	MyMoto_STPStartX(STPMT_2);
	MyMoto_STPSet(STPMT_2, 135, CCW, SS_High);
	MyMoto_STPStartX(STPMT_2);
}


void MyTossing_BlankingHome()
{	
	MyMTENC_Start(ENC_Blanking);
	MyMoto_STPSet(STPMT_1, 720, CW, SS_High);
	MyMoto_STPStartX(STPMT_1);
	MyMTENC_CheckHome();
	if (!MyMTENC_IsInHome(ENC_Blanking))
	{
		MyMoto_STPSet(STPMT_1, 720, CCW, SS_High);
		MyMoto_STPStartX(STPMT_1);
		
		if(!MyMTENC_IsInHome(ENC_Blanking))
		{
			Tossing_Error = Toss_BlankingHome_Error;
			MyMTENC_Stop(ENC_Blanking);
			return;
		}
	}		

	Tossing_Status = BlankingHome_Ready;	
	Tossing_Error = Toss_NoError;
	MyMTENC_Stop(ENC_Blanking);
}

void MyTossing_Init()
{

	MyMoto_Init();
	MyMTENC_Init();
	//MyPWM_Init();	
	Tossing_Status = Init_Ready;
	//MyTossing_BlankingHome();
    /*-----------------------------------------------------------------------------------------------------	*/
    /* Timer Init                                                                       					*/
    /*-----------------------------------------------------------------------------------------------------	*/
	//MyTimer_Init(STPMT_Timer,Counter_Mode,1);
	//MyTimer_Enable(STPMT_Timer);
    /*-----------------------------------------------------------------------------------------------------*/
    /* GPIO Interrupt Function Test                                                                        */
    /*-----------------------------------------------------------------------------------------------------*/	
}

#ifdef MyFunc_CLI

void MyTossing_CLI_GetErrorCode(void)
{
	MyTossing_GetErrorCode();
}

void MyTossing_CLI_GetStatus(void)
{
	MyTossing_GetStatus();
}



#endif
