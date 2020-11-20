/*
 * MyDev_LED_KTD2027.c
 *
 *  Created on: Oct 16, 2020
 *      Author: SeanChiu
 */

#include "MyDev_LED_KTD2027.h"


void MyLED_KTD2027_Init()
{
	MyI2C_Write(0x60, 0x00 ,0x06,A1D1);
	MyI2C_Write(0x60, 0x06 ,0x0A,A1D1);	// D1 Red
	MyI2C_Write(0x60, 0x07 ,0x05,A1D1);	// D2 Green
	MyI2C_Write(0x60, 0x08 ,0x14,A1D1);	// D3 Blue
	MyI2C_Write(0x60, 0x09 ,0x14,A1D1);	// D4 White
	My_Delay(50);
}

void MyLED_KTD2027_SetCurrent(uint16_t Current)
{
	MyI2C_Write(0x60, 0x06 ,Current,A1D1);
	MyI2C_Write(0x60, 0x07 ,Current,A1D1);
	MyI2C_Write(0x60, 0x08 ,Current,A1D1);
	MyI2C_Write(0x60, 0x09 ,Current,A1D1);
	My_Delay(50);
}

void MyLED_KTD2027_Off()
{
	MyI2C_Write(0x60, 0x04 ,0x00,A1D1);

}
void MyLED_KTD2027_White()
{
	MyI2C_Write(0x60, 0x04 ,0x00,A1D1);
	MyI2C_Write(0x60, 0x04 ,0x40,A1D1);
}

void MyLED_KTD2027_Red()
{
	MyI2C_Write(0x60, 0x04 ,0x00,A1D1);
	MyI2C_Write(0x60, 0x04 ,0x01,A1D1);
}

void MyLED_KTD2027_Green()
{
	MyI2C_Write(0x60, 0x04 ,0x00,A1D1);
	MyI2C_Write(0x60, 0x04 ,0x04,A1D1);
}

void MyLED_KTD2027_Blue()
{
	MyI2C_Write(0x60, 0x04 ,0x00,A1D1);
	MyI2C_Write(0x60, 0x04 ,0x10,A1D1);
}

void MyLED_KTD2027_Yellow()
{
	MyI2C_Write(0x60, 0x04 ,0x00,A1D1);
	MyI2C_Write(0x60, 0x04 ,0x05,A1D1);
}


void MyLED_KTD2027_BlueGreen()
{
	MyI2C_Write(0x60, 0x04 ,0x00,A1D1);
	MyI2C_Write(0x60, 0x04 ,0x14,A1D1);
}

void MyLED_KTD2027_Purple()
{
	MyI2C_Write(0x60, 0x04 ,0x00,A1D1);
	MyI2C_Write(0x60, 0x04 ,0x11,A1D1);
}
