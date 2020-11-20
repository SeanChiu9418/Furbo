/*
 * MyDev_LED_KTD2027.h
 *
 *  Created on: Oct 16, 2020
 *      Author: SeanChiu
 */

#ifndef CORE_INC_MYDEV_LED_KTD2027_H_
#define CORE_INC_MYDEV_LED_KTD2027_H_

#include "MyMCU_Inc.h"

void MyLED_KTD2027_Init();
void MyLED_KTD2027_Off();
void MyLED_KTD2027_White();
void MyLED_KTD2027_Red();
void MyLED_KTD2027_Green();
void MyLED_KTD2027_Yellow();
void MyLED_KTD2027_Blue();
void MyLED_KTD2027_Purple();
void MyLED_KTD2027_BlueGreen();
void MyLED_KTD2027_SetCurrent(uint16_t Current);

#endif /* CORE_INC_MYDEV_LED_KTD2027_H_ */
