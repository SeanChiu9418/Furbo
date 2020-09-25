/*
 * MyDev_Motor.c
 *
 *  Created on: Aug 12, 2020
 *      Author: SeanChiu
 */


#include <stdio.h>
#include "MyDev_Motor.h"

//#define StepsPerRev	4096	//Steps per revolution 360/STEP ANGLE(5.625)*gear ratio(64)
#define StepsPerRev	2048	//Steps per revolution 360/STEP ANGLE(5.625)*gear ratio(64)

static STPMT_Handle STPMT[STPMT_Num];

#define MyMotor_Delay(A) My_Delay(A)

void MyMoto_Init()
{	
	
	STPMT[STPMT_1].IN1.Port = 	STPMT1_IN1_Port;
	STPMT[STPMT_1].IN1.Pin 	= 	STPMT1_IN1_Pin;
	STPMT[STPMT_1].IN2.Port = 	STPMT1_IN2_Port;
	STPMT[STPMT_1].IN2.Pin 	= 	STPMT1_IN2_Pin;
	STPMT[STPMT_1].IN3.Port = 	STPMT1_IN3_Port;
	STPMT[STPMT_1].IN3.Pin 	= 	STPMT1_IN3_Pin;
	STPMT[STPMT_1].IN4.Port = 	STPMT1_IN4_Port;
	STPMT[STPMT_1].IN4.Pin 	= 	STPMT1_IN4_Pin;
	STPMT[STPMT_1].Angle	=	360;
	STPMT[STPMT_1].Direct	=	CW;
	STPMT[STPMT_1].Speed	=	SS_High;
	
	STPMT[STPMT_2].IN1.Port = 	STPMT_IN1_Port;
	STPMT[STPMT_2].IN1.Pin 	= 	STPMT_IN1_Pin;
	STPMT[STPMT_2].IN2.Port = 	STPMT_IN2_Port;
	STPMT[STPMT_2].IN2.Pin 	= 	STPMT_IN2_Pin;
	STPMT[STPMT_2].IN3.Port = 	STPMT_IN3_Port;
	STPMT[STPMT_2].IN3.Pin 	= 	STPMT_IN3_Pin;
	STPMT[STPMT_2].IN4.Port = 	STPMT_IN4_Port;
	STPMT[STPMT_2].IN4.Pin 	= 	STPMT_IN4_Pin;
	STPMT[STPMT_2].Angle	=	360;
	STPMT[STPMT_2].Direct	=	CW;
	STPMT[STPMT_2].Speed	=	SS_High;
	
#ifdef MCU_STM32
	/* GPIO Ports Clock Enable */
	MyGPIO_PortC_Enable();
#endif

	/*Configure GPIO pin Output Level */
	MyGPIO_WritePin(STPMT1_IN1_Port, STPMT1_IN1_Pin, GPIOPIN_RESET);
	MyGPIO_WritePin(STPMT1_IN2_Port, STPMT1_IN2_Pin, GPIOPIN_RESET);
	MyGPIO_WritePin(STPMT1_IN3_Port, STPMT1_IN3_Pin, GPIOPIN_RESET);
	MyGPIO_WritePin(STPMT1_IN4_Port, STPMT1_IN4_Pin, GPIOPIN_RESET);
	MyGPIO_WritePin(STPMT_IN1_Port, STPMT_IN1_Pin, GPIOPIN_RESET);
	MyGPIO_WritePin(STPMT_IN2_Port, STPMT_IN2_Pin, GPIOPIN_RESET);
	MyGPIO_WritePin(STPMT_IN3_Port, STPMT_IN3_Pin, GPIOPIN_RESET);
	MyGPIO_WritePin(STPMT_IN4_Port, STPMT_IN4_Pin, GPIOPIN_RESET);

	/*Configure GPIO pins : STPMT_IN1_Pin STPMT_IN2_Pin STPMT_IN3_Pin STPMT_IN4_Pin  */
	MyGPIO_Conf2Output(STPMT1_IN1_Port, STPMT1_IN1_Pin);
	MyGPIO_Conf2Output(STPMT1_IN2_Port, STPMT1_IN2_Pin);
	MyGPIO_Conf2Output(STPMT1_IN3_Port, STPMT1_IN3_Pin);
	MyGPIO_Conf2Output(STPMT1_IN4_Port, STPMT1_IN4_Pin);
	MyGPIO_Conf2Output(STPMT_IN1_Port, STPMT_IN1_Pin);
	MyGPIO_Conf2Output(STPMT_IN2_Port, STPMT_IN2_Pin);
	MyGPIO_Conf2Output(STPMT_IN3_Port, STPMT_IN3_Pin);
	MyGPIO_Conf2Output(STPMT_IN4_Port, STPMT_IN4_Pin);

}

//void MyMoto_SetSTPSpeed(int rpm)  // Set rpm--> max 13, min 1,,,  went to 14 rev/min
//{
	//MyMoto_Delay(60000000/StepsPerRev/rpm);
//}
void MyMoto_STPDriveEx(STPMT_No No, STPMT_Phase Phase)
{
	switch (Phase)
	{
	case STPMT_Phase0:
		MyGPIO_PinBitSet(STPMT[No].IN1.Port, STPMT[No].IN1.Pin);
		MyGPIO_PinBitSet(STPMT[No].IN2.Port, STPMT[No].IN2.Port);
		MyGPIO_PinBitReset(STPMT[No].IN3.Port, STPMT[No].IN3.Pin);
		MyGPIO_PinBitReset(STPMT[No].IN4.Port, STPMT[No].IN4.Pin);
		break;

	case STPMT_Phase1:
		MyGPIO_PinBitReset(STPMT[No].IN1.Port, STPMT[No].IN1.Pin);
		MyGPIO_PinBitSet(STPMT[No].IN2.Port, STPMT[No].IN2.Pin);
		MyGPIO_PinBitSet(STPMT[No].IN3.Port, STPMT[No].IN3.Pin);
		MyGPIO_PinBitReset(STPMT[No].IN4.Port, STPMT[No].IN4.Pin);
		break;

	case STPMT_Phase2:
		MyGPIO_PinBitReset(STPMT[No].IN1.Port, STPMT[No].IN1.Pin);
		MyGPIO_PinBitReset(STPMT[No].IN2.Port, STPMT[No].IN2.Pin);
		MyGPIO_PinBitSet(STPMT[No].IN3.Port, STPMT[No].IN3.Pin);
		MyGPIO_PinBitSet(STPMT[No].IN4.Port, STPMT[No].IN4.Pin);
		break;

	case STPMT_Phase3:
		MyGPIO_PinBitSet(STPMT[No].IN1.Port, STPMT[No].IN1.Pin);
		MyGPIO_PinBitReset(STPMT[No].IN2.Port, STPMT[No].IN2.Pin);
		MyGPIO_PinBitReset(STPMT[No].IN3.Port, STPMT[No].IN3.Pin);
		MyGPIO_PinBitSet(STPMT[No].IN4.Port, STPMT[No].IN4.Pin);
		break;

	default:
		MyGPIO_PinBitReset(STPMT[No].IN1.Port, STPMT[No].IN1.Pin);
		MyGPIO_PinBitReset(STPMT[No].IN2.Port, STPMT[No].IN2.Pin);
		MyGPIO_PinBitReset(STPMT[No].IN3.Port, STPMT[No].IN3.Pin);
		MyGPIO_PinBitReset(STPMT[No].IN4.Port, STPMT[No].IN4.Pin);
		break;
	}
}

void MyMoto_STPStartX(STPMT_No No)
{
	float AnglePerSequence = (float)360 / StepsPerRev;  // 360 = 4096 sequences
	int seq, NumberOfSequences = (int) (STPMT[No].Angle/AnglePerSequence);

	if (STPMT[No].Direct == CW)	// for clockwise
	{
		for (seq = 0; seq < NumberOfSequences; seq++)
		{
			if (STPMT[No].Angle == 0)	// Emergency Stop
			{
				printf("!!!!! Emergency Stop !!!!! \r\n");
				break;
			}
			else
			{
				MyMoto_STPDriveEx(No,seq%STPMT_Phase_Num);
				MyMotor_Delay(STPMT[No].Speed);
			}
		}
	}
	else	// for anti-clockwise
	{
		for (seq = NumberOfSequences; seq > 0; seq--)
		{
			if (STPMT[No].Angle == 0)	// Emergency Stop
			{	
				printf("!!!!! Emergency Stop !!!!! \r\n");
				break;
			}
			else
			{
				MyMoto_STPDriveEx(No,seq%STPMT_Phase_Num);
				MyMotor_Delay(STPMT[No].Speed);
			}
		}
	}
	MyMoto_STPDriveEx(No,STPMT_Phase_Num);	// close motor	
}

void MyMoto_STPDriver(int step)	
{
#if 1
	switch (step)
	{
	case 0:
		MyGPIO_PinBitSet(STPMT_IN1_Port, STPMT_IN1_Pin);
		MyGPIO_PinBitSet(STPMT_IN2_Port, STPMT_IN2_Pin);
		MyGPIO_PinBitReset(STPMT_IN3_Port, STPMT_IN3_Pin);
		MyGPIO_PinBitReset(STPMT_IN4_Port, STPMT_IN4_Pin);
		break;

	case 1:
		MyGPIO_PinBitReset(STPMT_IN1_Port, STPMT_IN1_Pin);
		MyGPIO_PinBitSet(STPMT_IN2_Port, STPMT_IN2_Pin);
		MyGPIO_PinBitSet(STPMT_IN3_Port, STPMT_IN3_Pin);
		MyGPIO_PinBitReset(STPMT_IN4_Port, STPMT_IN4_Pin);
		break;

	case 2:
		MyGPIO_PinBitReset(STPMT_IN1_Port, STPMT_IN1_Pin);
		MyGPIO_PinBitReset(STPMT_IN2_Port, STPMT_IN2_Pin);
		MyGPIO_PinBitSet(STPMT_IN3_Port, STPMT_IN3_Pin);
		MyGPIO_PinBitSet(STPMT_IN4_Port, STPMT_IN4_Pin);
		break;

	case 3:
		MyGPIO_PinBitSet(STPMT_IN1_Port, STPMT_IN1_Pin);
		MyGPIO_PinBitReset(STPMT_IN2_Port, STPMT_IN2_Pin);
		MyGPIO_PinBitReset(STPMT_IN3_Port, STPMT_IN3_Pin);
		MyGPIO_PinBitSet(STPMT_IN4_Port, STPMT_IN4_Pin);
		break;

	default:
		MyGPIO_PinBitReset(STPMT_IN1_Port, STPMT_IN1_Pin);
		MyGPIO_PinBitReset(STPMT_IN2_Port, STPMT_IN2_Pin);
		MyGPIO_PinBitReset(STPMT_IN3_Port, STPMT_IN3_Pin);
		MyGPIO_PinBitReset(STPMT_IN4_Port, STPMT_IN4_Pin);
		break;
	}
#else
	case 0:
		MyGPIO_WritePin(STPMT_IN1_Port, STPMT_IN1_Pin, GPIO_PIN_SET);
		MyGPIO_WritePin(STPMT_IN2_Port, STPMT_IN2_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN3_Port, STPMT_IN3_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN4_Port, STPMT_IN4_Pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(GPIOC, STPMT_IN1_Pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(GPIOC, STPMT_IN2_Pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(GPIOC, STPMT_IN3_Pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(GPIOC, STPMT_IN4_Pin, GPIO_PIN_RESET);
		break;

	case 1:
		MyGPIO_WritePin(STPMT_IN1_Port, STPMT_IN1_Pin, GPIO_PIN_SET);
		MyGPIO_WritePin(STPMT_IN2_Port, STPMT_IN2_Pin, GPIO_PIN_SET);
		MyGPIO_WritePin(STPMT_IN3_Port, STPMT_IN3_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN4_Port, STPMT_IN4_Pin, GPIO_PIN_RESET);
		break;

	case 2:
		MyGPIO_WritePin(STPMT_IN1_Port, STPMT_IN1_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN2_Port, STPMT_IN2_Pin, GPIO_PIN_SET);
		MyGPIO_WritePin(STPMT_IN3_Port, STPMT_IN3_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN4_Port, STPMT_IN4_Pin, GPIO_PIN_RESET);
		break;

	case 3:
		MyGPIO_WritePin(STPMT_IN1_Port, STPMT_IN1_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN2_Port, STPMT_IN2_Pin, GPIO_PIN_SET);
		MyGPIO_WritePin(STPMT_IN3_Port, STPMT_IN3_Pin, GPIO_PIN_SET);
		MyGPIO_WritePin(STPMT_IN4_Port, STPMT_IN4_Pin, GPIO_PIN_RESET);
		break;

	case 4:
		MyGPIO_WritePin(STPMT_IN1_Port, STPMT_IN1_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN2_Port, STPMT_IN2_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN3_Port, STPMT_IN3_Pin, GPIO_PIN_SET);
		MyGPIO_WritePin(STPMT_IN4_Port, STPMT_IN4_Pin, GPIO_PIN_RESET);
		break;

	case 5:
		MyGPIO_WritePin(STPMT_IN1_Port, STPMT_IN1_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN2_Port, STPMT_IN2_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN3_Port, STPMT_IN3_Pin, GPIO_PIN_SET);
		MyGPIO_WritePin(STPMT_IN4_Port, STPMT_IN4_Pin, GPIO_PIN_SET);
		break;

	case 6:
		MyGPIO_WritePin(STPMT_IN1_Port, STPMT_IN1_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN2_Port, STPMT_IN2_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN3_Port, STPMT_IN3_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN4_Port, STPMT_IN4_Pin, GPIO_PIN_SET);
		break;

	case 7:
		MyGPIO_WritePin(STPMT_IN1_Port, STPMT_IN1_Pin, GPIO_PIN_SET);
		MyGPIO_WritePin(STPMT_IN2_Port, STPMT_IN2_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN3_Port, STPMT_IN3_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN4_Port, STPMT_IN4_Pin, GPIO_PIN_SET);
		break;

	default:
		MyGPIO_WritePin(STPMT_IN1_Port, STPMT_IN1_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN2_Port, STPMT_IN2_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN3_Port, STPMT_IN3_Pin, GPIO_PIN_RESET);
		MyGPIO_WritePin(STPMT_IN4_Port, STPMT_IN4_Pin, GPIO_PIN_RESET);
		break;
	}
#endif
}

void MyMoto_STPStartEx(void)
{
	//MyMoto_STPStart(STP_Ang,Motor_Dir,STP_Speed);
	MyMoto_STPStartX(STPMT_1);
}

void MyMoto_STPStartCCEx(void)
{

}

void MyMoto_STPStartDemo(void)
{
	
	int i,j = (int)MyCLI_UnsignedCommandArgument(0);
	for(i = 0 ; i < j ; i ++)
	{
		//MyMoto_STPStart(STP_Ang,Motor_Dir,STP_Speed);
	}
}

void MyMoto_STPStart(int angle, MotorDirect direction, STPMT_Speed Speed)
{
	float AnglePerSequence = (float)360 / StepsPerRev;  // 360 = 4096 sequences
	int seq, NumberOfSequences = (int) (angle/AnglePerSequence);


	if (direction == CW)	// for clockwise
	{
		for (seq = 0; seq < NumberOfSequences; seq++)
		{
			MyMoto_STPDriver(seq%4);
			MyMotor_Delay(Speed);
		}
	}
	else	// for anti-clockwise
	{
		for (seq = NumberOfSequences; seq > 0; seq--)
		{
			MyMoto_STPDriver(seq%4);
			MyMotor_Delay(Speed);
		}
	}
	MyMoto_STPDriver(4);	// close motor
}

#ifdef MyFunc_CLI

void MyMoto_CLI_STPMTStart(void)
{
	STPMT_No No = (STPMT_No)MyCLI_UnsignedCommandArgument(0);
	if (No >= STPMT_Num)
	{
		printf("!!!!! STPMT_No Error !!!!! \r\n");
		return;
	}
	MyMoto_STPStartX(No);
}

void MyMoto_CLI_STPMTSet(void)
{
	STPMT_No No = (STPMT_No)MyCLI_UnsignedCommandArgument(0);
	if (No >= STPMT_Num)
	{
		printf("!!!!! STPMT_No Error !!!!! \r\n");
		return;
	}
	STPMT[No].Angle = MyCLI_UnsignedCommandArgument(1);
	STPMT[No].Direct = (MotorDirect)MyCLI_UnsignedCommandArgument(2);
	STPMT[No].Speed = (STPMT_Speed)(MyCLI_UnsignedCommandArgument(3) + SS_High);	// "+ SS_High" is for MAPPING
	printf("STP No.%d Angle:%d Direct:%d Speed:%d \r\n",No, STPMT[No].Angle, STPMT[No].Direct, (STPMT[No].Speed-SS_High));		
}

#endif



