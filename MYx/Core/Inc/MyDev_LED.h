/*
 * MyDev_LED.h
 *
 *  Created on: Jul 6, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MYDEV_LED_H_
#define INC_MYDEV_LED_H_

#include "MyMCU_Inc.h"



typedef enum
{
	LED_Off = 0,
	LED_On
}LEDOnOff;


void MyLED_Green(LEDOnOff ON);

#endif /* INC_MYDEV_LED_H_ */
