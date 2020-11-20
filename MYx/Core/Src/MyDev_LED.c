/*
 * MyDev_LED.c
 *
 *  Created on: Jul 6, 2020
 *      Author: SeanChiu
 */

#include "MyDev_LED.h"

void MyLED_Init()
{
#ifdef Dev_LED_KTD2027
	MyLED_KTD2027_Init();
#endif
}

void MyLED_Off()
{
#ifdef Dev_LED_KTD2027
	MyLED_KTD2027_Off();
#endif
}
void MyLED_OnOff(LEDColor Color, LEDOnOff OnOff)
{
	if(OnOff == LED_On)
		switch (Color)
		{
		case LED_White:
#ifdef Dev_LED_KTD2027
				MyLED_KTD2027_White();
#endif
			break;

		case LED_Red:
#ifdef Dev_LED_KTD2027
				MyLED_KTD2027_Red();
#endif
			break;

		case LED_Green:
#ifdef Dev_LED_KTD2027
				MyLED_KTD2027_Green();
#endif
			break;

		case LED_Yellow:
#ifdef Dev_LED_KTD2027
				MyLED_KTD2027_Yellow();
#endif
			break;

		case LED_Blue:
#ifdef Dev_LED_KTD2027
				MyLED_KTD2027_Blue();
#endif
			break;

		case LED_Purple:
#ifdef Dev_LED_KTD2027
			MyLED_KTD2027_Purple();
#endif
			break;

		case LED_BlueGreen:
#ifdef Dev_LED_KTD2027
			MyLED_KTD2027_BlueGreen();
#endif
			break;

		case LED_Color_Num:
		default:
			MyLED_Off();
			break;
		}
	else
		MyLED_Off();
}

void MyLED_Green(LEDOnOff OnOff)
{
	MyGPIO_WritePin(LED_G_Port,LED_G_Pin,OnOff);
}

#ifdef MyFunc_CLI

//#define MyFunc_CLI

void MyLED_CLI_On()
{
	LEDColor Color;

	Color = (LEDColor)MyCLI_UnsignedCommandArgument(0);
	MyLED_OnOff(Color, LED_On);
}

void MyLED_CLI_SetBrightness()
{
	uint16_t Brightness;
	Brightness = (uint16_t)MyCLI_UnsignedCommandArgument(0);
	MyLED_KTD2027_SetCurrent(Brightness);
}

void MyLED_CLI_Test()
{
#ifdef Dev_LED_KTD2027
	MyLED_KTD2027_Init();
	while(1)
	{
		  MyLED_OnOff(LED_Red, LED_On);
		  My_Delay(5000);
		  MyLED_OnOff(LED_Red, LED_Off);
		  My_Delay(5000);
		  MyLED_OnOff(LED_Green, LED_On);
		  My_Delay(5000);
		  MyLED_OnOff(LED_Green, LED_Off);
		  My_Delay(5000);
		  MyLED_OnOff(LED_Blue, LED_On);
		  My_Delay(5000);
		  MyLED_OnOff(LED_Blue, LED_Off);
		  My_Delay(5000);
		  MyLED_OnOff(LED_White, LED_On);
		  My_Delay(5000);
		  MyLED_OnOff(LED_White, LED_Off);
		  My_Delay(5000);
	}

#endif
}
#endif

