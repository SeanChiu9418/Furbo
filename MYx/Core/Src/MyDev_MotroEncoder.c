/*
 * MyDev_MotroEncoder.c
 *
 *  Created on: Oct 5, 2020
 *      Author: SeanChiu
 */
#include <stdio.h>
#include "MyDev_MotorEncoder.h"

static char Is_InHome; //Bit0: Blanking, Bit1:Throw, Bit2:PTZ 
static char ENC_IntDect; // Interrupt Detect 0: Not Interrupt yet, 1:Interrupted
static MTENC_Status ENC_DC_Status;
static int IntCnt = 0; //Interruper_Cnt
Timer_Index MTENC[ENC_NUM] = {Timer_0, Timer_1, Timer_2};
GPIO_INT_Index MTENC_GPIO[ENC_NUM] = {GPIO_INT0, GPIO_INT1, GPIO_INT2};

#define SkipCnt 100
#define AvgBufCnt (40 + SkipCnt)
#define StableCnt (10 + AvgBufCnt)
#define StableThreshold 10 // unit % for check stable or not
#define ThrowingThreshold 20 // unit % for check stable or not


void MyMTENC_Init()
{
	Is_InHome = 0;
	ENC_DC_Status = ENC_Start;
	
	//MyTimer_Init(MTENC[ENC_Blanking] , Counter_Mode ,1);	// Init Timer    
	//MyTimer_EnableInt(MTENC[ENC_Blanking]);	// Enable timer interrupt    
    //MyTimer_Start(STPMT1_Timer);	// Start Timer

	MyTimer_Init(MTENC[ENC_Throw] , Counter_Mode ,1);	// Init Timer    
	MyTimer_EnableInt(MTENC[ENC_Throw]);	// Enable timer interrupt    
    //MyTimer_Start(DCMT_Timer);	// Start Timer	
	
    /* Configure PB.5 as Input mode and enable interrupt by rising edge trigger */
	//MyGPIO_Conf2INT_RISING(STPMT1_ENC_Port,STPMT1_ENC_Pin);
	GPIO_INT[MTENC_GPIO[ENC_Blanking]].Port = STPMT1_ENC_Port;
	GPIO_INT[MTENC_GPIO[ENC_Blanking]].Pin = STPMT1_ENC_Pin;
	GPIO_INT[MTENC_GPIO[ENC_Blanking]].Type = INT_RISING;

    /* Configure PB.4 as Input mode and enable interrupt by rising edge trigger */
	//MyGPIO_Conf2INT_RISING(DCMT_ENC_Port,DCMT_ENC_Pin);	
	GPIO_INT[MTENC_GPIO[ENC_Throw]].Port = DCMT_ENC_Port;
	GPIO_INT[MTENC_GPIO[ENC_Throw]].Pin = DCMT_ENC_Pin;
	GPIO_INT[MTENC_GPIO[ENC_Throw]].Type = INT_RISING;
	
    /* Configure PB.2 as Input mode and enable interrupt by rising edge trigger */
	//MyGPIO_Conf2INT_RISING(STPMT2_ENC_Port,STPMT2_ENC_Pin);		
	GPIO_INT[MTENC_GPIO[ENC_PTZ]].Port = STPMT2_ENC_Port;
	GPIO_INT[MTENC_GPIO[ENC_PTZ]].Pin = STPMT2_ENC_Pin;
	GPIO_INT[MTENC_GPIO[ENC_PTZ]].Type = INT_RISING;
	//MyGPIO_EnableINT(ENC_Blanking);
	MyNVIC_EnableIRQ(GPB_IRQn);
	
	// Check ENC in Home Position or not
	MyMTENC_CheckHome();
	/* Enable interrupt de-bounce function*/
	MyGPIO_DeBounce();	
}

void MyMTENC_Start(MTENC_Index IDX)
{	
	MyGPIO_EnableINT(MTENC_GPIO[IDX]);
    MyTimer_Start(MTENC[IDX]);	// Start Timer	
}
	
void MyMTENC_Stop(MTENC_Index IDX)
{	
	MyGPIO_DisableINT(MTENC_GPIO[IDX]);
    MyTimer_Stop(MTENC[IDX]);	// Stop Timer	
}

void MyMTENC_NVIC_DCTimeOut()
{
	ENC_DC_Status = ENC_TimeOut;
	printf("!!!!! DC ENC TimeOut !!!!! \r\n");
}

void MyMTENC_NVIC_STPMT1()	// STPMT1_ENC_Pin Interrupt
{
	MyMoto_STPStop(STPMT_1);
	MyGPIO_DisableINT(MTENC_GPIO[ENC_Blanking]);
}

void MyMTENC_NVIC_DCMT(uint32_t timer_cnt)	// STPMT1_ENC_Pin Interrupt
{
	static int avg,min,max,throwing;	//  & Threshold
	static uint32_t timer_buf =0;
	IntCnt++;
	if (IntCnt <= SkipCnt) // at the first cnts, the timer_cnt isn't stable. skip it
		return;
	else
	{
		if(IntCnt < AvgBufCnt)	// sum the timer_cnt for avg
		{
			timer_buf += timer_cnt;
		}
		else if(IntCnt == AvgBufCnt)
		{
			avg = timer_buf / (AvgBufCnt - SkipCnt);
			max = avg + (avg * StableThreshold / 100);
			min = avg - (avg * StableThreshold / 100);
			printf("***** DC_ENC avg =%d, max=%d, min=%d *****\r\n",avg,max,min);
		}
		else if(IntCnt < StableCnt)
		{			
			//printf("5555 cnt=%d timer_cnt=%d \r\n",cnt,timer_cnt);
			if((timer_cnt > max)||(timer_cnt < min))
			{
				printf("!!!!! DC_ENC Unsable timer_cnt=%d !!!!!r\n",timer_cnt);
				ENC_DC_Status = ENC_UnStable;
				return;
			}
		}else if (IntCnt == StableCnt)
		{
			ENC_DC_Status = ENC_Stabled;
			throwing = avg + (avg * ThrowingThreshold / 100);
			printf("***** DC_ENC ThrowThreshold=%d *****\r\n",throwing);
			timer_buf = 0;
		}
		else
		{
			if(timer_cnt >= throwing)
			{
				ENC_DC_Status = ENC_GetInterrupt;
				ENC_IntDect = true;
				//printf("***** DC_ENC GetThrowing CNT=%d *****\r\n",timer_cnt);	
				timer_buf = 0;  
			} 
			else
			{
				if((timer_cnt < max)&&(timer_cnt > min))
				{
					timer_buf ++;
					if (timer_buf >= (StableCnt - AvgBufCnt))
						ENC_DC_Status = ENC_Stabled;
				}
			}
			
		}
	}
}

void MyMTENC_CheckHome()
{
	MTENC_Index index;
	for (index = ENC_Blanking; index < ENC_NUM; index ++)
	{
		if(MyGPIO_PinInputValue(GPIO_INT[MTENC_GPIO[index]].Port, GPIO_INT[MTENC_GPIO[index]].Pin) == GPIOPIN_SET)		
			MyMTENC_InHome(index, true);
		else		
			MyMTENC_InHome(index, false);

	}
	//printf("***** Is_InHome= %d *****\n", Is_InHome);

}

void MyMTENC_InHome(MTENC_Index index, int IsHome)
{
	Is_InHome &=  ~((0x0 | (0x1 << index)));
	Is_InHome |=  (IsHome << index);

}	
	
int MyMTENC_IsInHome(MTENC_Index index)
{
	if((Is_InHome >> index) & 0x01)
		return true;
	else
		return false;	
}


int MyMTENC_GetDCStatus()
{
	return ENC_DC_Status;
}


void MyMTENC_SetDCStatus(MTENC_Status status)
{
	ENC_DC_Status = status;
}


void MyMTENC_SetInterruptCnt(int cnt)
{
	IntCnt = cnt;
}

char MyMTENC_IsIntDetect()
{
	return ENC_IntDect;
}

void MyMTENC_ClearIntDetect()
{
	ENC_IntDect = 0;
}

#ifdef MyFunc_CLI

void MyMTENC_CLI_Init()
{
	MyMTENC_Init();
	MyMTENC_Start(ENC_Throw);
}

#endif

