/*
 * MyMCU_I2C.c
 *
 *  Created on: Sep 18, 2020
 *      Author: SeanChiu
 */

#include <stdio.h>
#include "MyMCU_I2C.h"

#ifdef MCU_STM32
I2C_HandleTypeDef hi2c2;
#endif

void MyI2C_Init()
{

#ifdef MCU_STM32

		  /****** USART1_Init 1 ******/
	  hi2c2.Instance = I2C2;
	  hi2c2.Init.Timing = 0x00303D5B;
	  hi2c2.Init.OwnAddress1 = 0;
	  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  hi2c2.Init.OwnAddress2 = 0;
	  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
	  {
	    Error_Handler();
	  }


	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	    __HAL_RCC_GPIOA_CLK_ENABLE();
	    /**I2C2 GPIO Configuration
	    PA11 [PA9]     ------> I2C2_SCL
	    PA12 [PA10]     ------> I2C2_SDA
	    */
	    //GPIO_InitStruct.Pin = GPIO_Pin[I2C_SCL_Pin]|GPIO_Pin[I2C_SDA_Pin];
	    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	    GPIO_InitStruct.Pull = GPIO_PULLUP;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    GPIO_InitStruct.Alternate = GPIO_AF6_I2C2;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	    /* Peripheral clock enable */
	    __HAL_RCC_I2C2_CLK_ENABLE();

	  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  My_Delay(1000);

#endif

}
char MyI2C_Write(short SlaveID, short address, short value, I2CType Type)
{
#ifdef MCU_STM32
	char buf[4];
	HAL_StatusTypeDef ret;
	if (Type == A1D1)
	{
		buf[0] = (char)address;
		buf[1] = (char)value;
	}
	ret = HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)SlaveID, (uint8_t*)buf, 2, HAL_MAX_DELAY);

    if ( ret != HAL_OK )
    {
    	My_printf("!!!!! I2C TX Error !!!!!");
    	return false;
    }
	return true;
#endif	
	return 0;
}

char MyI2C_Read(short SlaveID, short address, short * value, I2CType Type)
{
#ifdef MCU_STM32
	char buf[4];
	int WriteSize, ReadSize;
	HAL_StatusTypeDef ret;
	switch (Type)
	{
	case A1D2:
		buf[0] = address;
		WriteSize = 1; // 1byte
		ReadSize = 2; // 2byte
		break;
	case A1D1:
	default:
		buf[0] = address;
		WriteSize = 1; // 1byte
		ReadSize = 1; // 1byte
		break;
	}

	ret = HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)SlaveID, (uint8_t*)buf, WriteSize, HAL_MAX_DELAY);
	if (ret != HAL_OK)
	{
		My_printf("!!!!! I2C TX Error !!!!!");
		return false;
	}
	My_Delay(10);

	ret = HAL_I2C_Master_Receive(&hi2c2, (uint16_t)SlaveID, (uint8_t*)buf, ReadSize, HAL_MAX_DELAY);
	if (ret != HAL_OK)
	{
		My_printf("!!!!! I2C TX Error !!!!!");
		return false;
	}

	switch (Type)
	{
	case A1D2:
		*value = ((buf[0] << 8) | buf[1]);
		break;
	case A1D1:
	default:
		*value = buf[0];
		break;
	}

	return true;
#endif		
	return 0;
}

void MyI2C_SelfTest()
{
	short SlaveID = 0xC0, address = 0x01, Value;
	I2CType Type = A1D2;
	MyI2C_Read(SlaveID, address, &Value, Type);
	printf("MyI2C_SelfTest = %d \r\n", Value);

}


void MyI2C_Test()
{
#ifdef STM32	
	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	    __HAL_RCC_GPIOA_CLK_ENABLE();
	    /**I2C2 GPIO Configuration
	    PA11 [PA9]     ------> I2C2_SCL
	    PA12 [PA10]     ------> I2C2_SDA
	    */
	    //GPIO_InitStruct.Pin = GPIO_Pin[I2C_SCL_Pin]|GPIO_Pin[I2C_SDA_Pin];
	    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	    GPIO_InitStruct.Pull = GPIO_PULLUP;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    GPIO_InitStruct.Alternate = GPIO_AF6_I2C2;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	    /* Peripheral clock enable */
	    __HAL_RCC_I2C2_CLK_ENABLE();
#endif

}
