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


void MyMoto_DC_Forward()
{
	printf("~~~~~ MyMoto_DC_Forward  ~~~~~ \r\n");
	MyGPIO_WritePin(DCMT_EN_Port, DCMT_EN_Pin, GPIOPIN_SET);
	
	MyPWM_StartOutput(DC_INT1_PWM);
	//MyGPIO_WritePin(DCMT_INT1_Port, DCMT_INT1_Pin, GPIOPIN_SET);	
	
	MyPWM_StopOutput(DC_INT2_PWM);
	//MyGPIO_WritePin(DCMT_INT2_Port, DCMT_INT2_Pin, GPIOPIN_RESET);
}


void MyMoto_DC_Reverse()
{	
	printf("~~~~~ MyMoto_DC_Reverse  ~~~~~ \r\n");
	MyGPIO_WritePin(DCMT_EN_Port, DCMT_EN_Pin, GPIOPIN_SET);
	
	MyPWM_StopOutput(DC_INT1_PWM);
	//MyGPIO_WritePin(DCMT_INT1_Port, DCMT_INT1_Pin, GPIOPIN_RESET);
	
	MyPWM_StartOutput(DC_INT2_PWM);
	//MyGPIO_WritePin(DCMT_INT2_Port, DCMT_INT2_Pin, GPIOPIN_SET);
}

void MyMoto_DC_Brake()
{	
	printf("~~~~~ MyMoto_DC_Brake  ~~~~~ \r\n");
	MyGPIO_WritePin(DCMT_EN_Port, DCMT_EN_Pin, GPIOPIN_SET);
	
	MyPWM_StopOutput(DC_INT1_PWM);
	//MyGPIO_WritePin(DCMT_INT1_Port, DCMT_INT1_Pin, GPIOPIN_SET);
	
	MyPWM_StopOutput(DC_INT2_PWM);
	//MyGPIO_WritePin(DCMT_INT2_Port, DCMT_INT2_Pin, GPIOPIN_SET);
	
}

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
	
	MyGPIO_WritePin(DCMT_EN_Port, DCMT_EN_Pin, GPIOPIN_RESET);	// for DC Motor Enable Pin
	//MyGPIO_WritePin(DCMT_INT1_Port, DCMT_INT1_Pin, GPIOPIN_RESET);	// for DC Motor INT1 Pin (PWM0_CH4)
	//MyGPIO_WritePin(DCMT_INT2_Port, DCMT_INT2_Pin, GPIOPIN_RESET);	// for DC Motor INT1 Pin (PWM0_CH5)

	/*Configure GPIO pins : STPMT_IN1_Pin STPMT_IN2_Pin STPMT_IN3_Pin STPMT_IN4_Pin  */
	MyGPIO_Conf2Output(STPMT1_IN1_Port, STPMT1_IN1_Pin);
	MyGPIO_Conf2Output(STPMT1_IN2_Port, STPMT1_IN2_Pin);
	MyGPIO_Conf2Output(STPMT1_IN3_Port, STPMT1_IN3_Pin);
	MyGPIO_Conf2Output(STPMT1_IN4_Port, STPMT1_IN4_Pin);
	MyGPIO_Conf2Output(STPMT_IN1_Port, STPMT_IN1_Pin);
	MyGPIO_Conf2Output(STPMT_IN2_Port, STPMT_IN2_Pin);
	MyGPIO_Conf2Output(STPMT_IN3_Port, STPMT_IN3_Pin);
	MyGPIO_Conf2Output(STPMT_IN4_Port, STPMT_IN4_Pin);

	MyGPIO_Conf2Output(DCMT_EN_Port, DCMT_EN_Pin);	// for DC Motor Enable Pin
	//MyGPIO_Conf2Output(DCMT_INT1_Port, DCMT_INT1_Pin);	// for DC Motor INT1 Pin (PWM0_CH4)
	//MyGPIO_Conf2Output(DCMT_INT2_Port, DCMT_INT2_Pin);	// for DC Motor INT1 Pin (PWM0_CH5)
	
	MyPWM_Init();
	MyPWM_Config(DC_INT1_PWM, 20000, 90);
	MyPWM_Config(DC_INT2_PWM, 20000, 90);	
	MyPWM_Eanble(DC_INT1_PWM);	
	MyPWM_Eanble(DC_INT2_PWM);
	//MyPWM_Start(DC_INT1_PWM);
	//MyPWM_Start(DC_INT2_PWM);
}


void MyMoto_STPDriveEx(STPMT_No No, STPMT_Phase Phase)
{
	switch (Phase)
	{
	case STPMT_Phase0:
		MyGPIO_PinBitReset(STPMT[No].IN1.Port, STPMT[No].IN1.Pin);
		MyGPIO_PinBitReset(STPMT[No].IN2.Port, STPMT[No].IN2.Pin);
		MyGPIO_PinBitSet(STPMT[No].IN3.Port, STPMT[No].IN3.Pin);
		MyGPIO_PinBitSet(STPMT[No].IN4.Port, STPMT[No].IN4.Pin);
		
		break;

	case STPMT_Phase1:
		MyGPIO_PinBitSet(STPMT[No].IN1.Port, STPMT[No].IN1.Pin);
		MyGPIO_PinBitReset(STPMT[No].IN2.Port, STPMT[No].IN2.Pin);
		MyGPIO_PinBitReset(STPMT[No].IN3.Port, STPMT[No].IN3.Pin);
		MyGPIO_PinBitSet(STPMT[No].IN4.Port, STPMT[No].IN4.Pin);
		break;

	case STPMT_Phase2:
		MyGPIO_PinBitSet(STPMT[No].IN1.Port, STPMT[No].IN1.Pin);
		MyGPIO_PinBitSet(STPMT[No].IN2.Port, STPMT[No].IN2.Port);
		MyGPIO_PinBitReset(STPMT[No].IN3.Port, STPMT[No].IN3.Pin);
		MyGPIO_PinBitReset(STPMT[No].IN4.Port, STPMT[No].IN4.Pin);
		break;

	case STPMT_Phase3:
		MyGPIO_PinBitReset(STPMT[No].IN1.Port, STPMT[No].IN1.Pin);
		MyGPIO_PinBitSet(STPMT[No].IN2.Port, STPMT[No].IN2.Pin);
		MyGPIO_PinBitSet(STPMT[No].IN3.Port, STPMT[No].IN3.Pin);
		MyGPIO_PinBitReset(STPMT[No].IN4.Port, STPMT[No].IN4.Pin);
		break;

	default:
		MyGPIO_PinBitReset(STPMT[No].IN1.Port, STPMT[No].IN1.Pin);
		MyGPIO_PinBitReset(STPMT[No].IN2.Port, STPMT[No].IN2.Pin);
		MyGPIO_PinBitReset(STPMT[No].IN3.Port, STPMT[No].IN3.Pin);
		MyGPIO_PinBitReset(STPMT[No].IN4.Port, STPMT[No].IN4.Pin);
		break;
	}
}

/*
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
*/

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
				printf("~~~~ STPMT Stop ~~~~ \r\n");
				break;
			}
			else
			{
				MyMoto_STPDriveEx(No,seq%STPMT_Phase_Num);
				MyMotor_Delay(STPMT[No].Speed);
				My_DelayUs(600) ;
			}
		}
	}
	else	// for anti-clockwise
	{
		for (seq = NumberOfSequences; seq > 0; seq--)
		{
			if (STPMT[No].Angle == 0)	// Emergency Stop
			{	
				printf("~~~~ STPMT Stop ~~~~~ \r\n");
				break;
			}
			else
			{
				MyMoto_STPDriveEx(No,seq%STPMT_Phase_Num);
				MyMotor_Delay(STPMT[No].Speed);
				My_DelayUs(600) ;
			}
		}
	}
	MyMoto_STPDriveEx(No,STPMT_Phase_Num);	// close motor	
}

void MyMoto_STPSet(STPMT_No no, int angle, MotorDirect direct,STPMT_Speed speed)
{
	STPMT[no].Angle = angle;
	STPMT[no].Direct = direct;
	STPMT[no].Speed = speed;	
}

/*
void MyMoto_STPStartEx(void)
{
	//MyMoto_STPStart(STP_Ang,Motor_Dir,STP_Speed);
	MyMoto_STPStartX(STPMT_1);
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
*/

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

void MyMoto_STPStop(STPMT_No no)
{
	STPMT[no].Angle = 0;
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
	STPMT_No no = (STPMT_No)MyCLI_UnsignedCommandArgument(0);
	if (no >= STPMT_Num)
	{
		printf("!!!!! STPMT_No Error !!!!! \r\n");
		return;
	}
	int angle = MyCLI_UnsignedCommandArgument(1);
	MotorDirect direct = (MotorDirect)MyCLI_UnsignedCommandArgument(2);
	STPMT_Speed speed = (STPMT_Speed)(MyCLI_UnsignedCommandArgument(3));	
	MyMoto_STPSet(no, angle, direct, speed + SS_High);	// "+ SS_High" is for MAPPING
	printf("STP No.%d Angle:%d Direct:%d Speed:%d \r\n",no, angle,direct, speed);		
}

void MyMoto_CLI_STPStop(void)
{
	STPMT_No no = (STPMT_No)MyCLI_UnsignedCommandArgument(0);
	if (no >= STPMT_Num)
	{
		printf("!!!!! STPMT_No Error !!!!! \r\n");
		return;
	}
	printf("~~~~~~~ MyMoto_CLI_STPStop ~~~~~~ \r\n");
	MyMoto_STPStop(no);
}

#endif



