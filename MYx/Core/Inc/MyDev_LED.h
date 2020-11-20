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


typedef enum
{
	LED_White = 0,
	LED_Red,
	LED_Green,
	LED_Blue,
	LED_Yellow,
	LED_Purple,
	LED_BlueGreen,
	LED_Color_Num
}LEDColor;

void MyLED_Init();
void MyLED_Off();
void MyLED_Green(LEDOnOff ON);
void MyLED_OnOff(LEDColor Color, LEDOnOff OnOff);

#ifdef MyFunc_CLI

void MyLED_CLI_On();
void MyLED_CLI_Test();
void MyLED_CLI_SetBrightness();
#define LED_CLI_TABLE \
{ "~~~~~ LED_CLI_TABLE Start ~~~~~", MyLED_Init, "" , "" },  \
{ "LED_I",      MyLED_Init, "" , "	: LED init" },  \
{ "LED_SB",      MyLED_CLI_SetBrightness, "u" , "	: Set LED Brightness \r\n	Brightness: 0(min)~255(max) \r\n	ex: LED_SB 128"},  \
{ "LED_On",      MyLED_CLI_On,	"u" , "	: LED (Color) On. \r\n	Color 0:White 1:Red 2:Green 3:Blue 4:Yellow 5:Purple 6:BlueGreen \r\n	ex:\"LED_On 0" },\
{ "LED_Off",      MyLED_Off,	"" , "	: LED Off" },\
{ "LED_Test",      MyLED_CLI_Test,	"" , "	: LED Test" },\
{ "~~~~~ LED_CLI_TABLE End ~~~~~", MyLED_Init, "", "" }


#endif

#endif /* INC_MYDEV_LED_H_ */
