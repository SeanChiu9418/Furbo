/*
 * MyDev_LED.c
 *
 *  Created on: Jul 6, 2020
 *      Author: SeanChiu
 */

#include "MyDev_LED.h"


void MyLED_Green(LEDOnOff OnOff)
{
	if (OnOff == LED_Off)
		MyGPIO_WritePin(LED_G_Port,LED_G_Pin,GPIOPIN_RESET);
	else
		MyGPIO_WritePin(LED_G_Port,LED_G_Pin,GPIOPIN_SET);
}


