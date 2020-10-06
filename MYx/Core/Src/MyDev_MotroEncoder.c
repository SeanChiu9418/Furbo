/*
 * MyDev_MotroEncoder.c
 *
 *  Created on: Oct 5, 2020
 *      Author: SeanChiu
 */

#include "MyDev_MotorEncoder.h"

static int DCMT_EncCnt;	//DC Motor Encoder Count

void MyMTENC_Init()
{
	
	//MyTimer_Init(STPMT1_Timer , Counter_Mode ,1);	// Init Timer    
	//MyTimer_EnableInt(STPMT1_Timer);	// Enable timer interrupt    
    //MyTimer_Start(STPMT1_Timer);	// Start Timer

	MyTimer_Init(DCMT_Timer , Counter_Mode ,1);	// Init Timer    
	//MyTimer_EnableInt(STPMT_Timer);	// Enable timer interrupt    
    MyTimer_Start(DCMT_Timer);	// Start Timer	
	
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


