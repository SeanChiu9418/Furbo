/*
 * MyDev_MotroEncoder.c
 *
 *  Created on: Oct 5, 2020
 *      Author: SeanChiu
 */

#include "MyDev_MotorEncoder.h"

static int DCMT_EncCnt;	//DC Motor Encoder Count

Timer_Index MTENC[ENC_NUM]= {Timer_0, Timer_1};

void MyMTENC_Init()
{
	
	MyTimer_Init(MTENC[ENC_STPMT1] , Counter_Mode ,1);	// Init Timer    
	MyTimer_EnableInt(MTENC[ENC_STPMT1]);	// Enable timer interrupt    
    //MyTimer_Start(STPMT1_Timer);	// Start Timer

	MyTimer_Init(MTENC[ENC_DCMT] , Counter_Mode ,1);	// Init Timer    
	MyTimer_EnableInt(MTENC[ENC_DCMT]);	// Enable timer interrupt    
    //MyTimer_Start(DCMT_Timer);	// Start Timer	
	
    /* Configure PB.5 as Input mode and enable interrupt by rising edge trigger */
	MyGPIO_Conf2INT_RISING(STPMT1_ENC_Port,STPMT1_ENC_Pin);
    /* Configure PB.4 as Input mode and enable interrupt by rising edge trigger */
	MyGPIO_Conf2INT_RISING(DCMT_ENC_Port,DCMT_ENC_Pin);	
    /* Configure PB.2 as Input mode and enable interrupt by rising edge trigger */
	MyGPIO_Conf2INT_RISING(STPMT2_ENC_Port,STPMT2_ENC_Pin);
	MyNVIC_EnableIRQ(GPB_IRQn);
	
	/* Enable interrupt de-bounce function*/
	MyGPIO_DeBounce();	
}

void MyMTENC_Start(MTENC_Index IDX)
{	
    MyTimer_Start(MTENC[IDX]);	// Start Timer	
}
	
void MyMTENC_Stop(MTENC_Index IDX)
{	
    MyTimer_Stop(MTENC[IDX]);	// Stop Timer	
}

#ifdef MyFunc_CLI

void MyMTENC_CLI_Init()
{
	MyMTENC_Init();
	MyMTENC_Start(ENC_DCMT);
}

#endif

