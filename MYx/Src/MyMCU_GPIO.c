/*
 * MyMCU_GPIO.c
 *
 *  Created on: Jul 6, 2020
 *      Author: SeanChiu
 */

#include "MyMCU_GPIO.h"

#ifdef MCU_STM32	

	GPIO_TypeDef *GPIO_Port[GPIO_Num_Port] = {GPIOA,GPIOB,GPIOC,GPIOD};
	uint16_t GPIO_Pin[GPIO_Num_Pin] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5,
									GPIO_PIN_6,	GPIO_PIN_7,	GPIO_PIN_8,	GPIO_PIN_9,GPIO_PIN_10,GPIO_PIN_11,
									GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_14,GPIO_PIN_15,GPIO_PIN_All,};
#elif defined(MCU_Nuvoton)

	GPIO_T *GPIO_Port[GPIO_Num_Port] = {PA,PB,PC,PD};
	uint16_t GPIO_Pin[GPIO_Num_Pin] = {BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7, 
									BIT8, BIT9, BIT10, BIT11, BIT12, BIT13, BIT14, BIT15,};	
#endif	


void MyGPIO_Init()
{
	
#ifdef MCU_STM32	

	/* GPIO Ports Clock Enable */
	  MyGPIO_PortC_Enable();
	  MyGPIO_PortD_Enable();

	  /*Configure GPIO pin Output Level */
	  // GPIOD, GPIO_PIN_8 -- My_Test1_Pin
	  // GPIOD, GPIO_PIN_9 -- My_Test2_Pin
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

	  /*Configure GPIO pin : My_Test1_Pin & My_Test2_Pin*/
	  MyGPIO_Conf2Output(GPIO_Port[GPIO_PD], My_Test1_Pin|My_Test2_Pin);

	  /*Configure GPIO pin : Button_B1_Pin */
	  MyGPIO_Conf2Input(GPIO_Port[GPIO_PC],Button_B1_Pin);
	
#elif defined(MCU_Nuvoton)

	//GPIO_SetMode(PB, BIT3, GPIO_MODE_OUTPUT);
	//GPIO_SetMode(PC, BIT0, GPIO_MODE_OUTPUT);
	MyGPIO_Conf2Output(My_Test1_Port,My_Test1_Pin);
	MyGPIO_Conf2Output(My_Test2_Port,My_Test2_Pin);
	MyGPIO_PinBitReset(My_Test1_Port,My_Test1_Pin);
	MyGPIO_PinBitReset(My_Test2_Port,My_Test2_Pin);

	MyGPIO_Conf2Input(My_Test3_Port,My_Test3_Pin);
	

#endif	

}



void MyGPIO_Conf2Output(GPIO_Port_Index Port, GPIO_Pin_Index Pin)
{ 	
#ifdef MCU_STM32		
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin[Pin];
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIO_Port[Port], &GPIO_InitStruct);
	
#elif defined(MCU_Nuvoton)

	GPIO_SetMode(GPIO_Port[Port], GPIO_Pin[Pin], GPIO_MODE_OUTPUT);
	
#endif	
}

void MyGPIO_Conf2Input(GPIO_Port_Index Port, GPIO_Pin_Index Pin)
{ 	
#ifdef MCU_STM32		
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};		
	GPIO_InitStruct.Pin = GPIO_Pin[Pin];					
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;		
	GPIO_InitStruct.Pull = GPIO_NOPULL;			
	HAL_GPIO_Init(GPIO_Port[Port], &GPIO_InitStruct);			
	
#elif defined(MCU_Nuvoton)

	GPIO_SetMode(GPIO_Port[Port], GPIO_Pin[Pin], GPIO_MODE_INPUT);
	
#endif	
}





GPIOPin_State MyGPIO_ReadPin(GPIO_Port_Index Port, GPIO_Pin_Index Pin)
{
#ifdef MCU_STM32
	
	return (GPIOPin_State)HAL_GPIO_ReadPin(GPIO_Port[Port], GPIO_Pin[Pin]);
	
#elif defined(MCU_Nuvoton)
	
	return MyGPIO_PinInputValue(Port, Pin);
	
#endif	
}

void MyGPIO_WritePin(GPIO_Port_Index Port, GPIO_Pin_Index Pin, GPIOPin_State State)
{
#ifdef MCU_STM32
	
	HAL_GPIO_WritePin(GPIO_Port[Port], GPIO_Pin[Pin], State);
	
#elif defined(MCU_Nuvoton)
	
	if (State == GPIOPIN_SET)
		MyGPIO_PinBitSet(Port,Pin);
	else
		MyGPIO_PinBitReset(Port,Pin);	
	
#endif
}

#ifdef MyFunc_CLI
void MyGPIO_CLI_WritePin()
{
	GPIO_Port_Index Port = (GPIO_Port_Index)MyCLI_UnsignedCommandArgument(0);
	GPIO_Pin_Index Pin = (GPIO_Pin_Index)MyCLI_UnsignedCommandArgument(1);
	GPIOPin_State State = (GPIOPin_State)MyCLI_UnsignedCommandArgument(2);

	My_printf(int2str((int)State));

	MyGPIO_WritePin(Port,Pin,State);
}

void MyGPIO_CLI_ReadPin()
{
	int result;
	GPIO_Port_Index Port = (GPIO_Port_Index)MyCLI_UnsignedCommandArgument(0);
	GPIO_Pin_Index Pin = (GPIO_Pin_Index)MyCLI_UnsignedCommandArgument(1);

	result = MyGPIO_ReadPin(Port,Pin);

	My_printf(int2str(result));

}

void MyGPIO_CLI_Set2InputMode()
{
	GPIO_Port_Index Port = (GPIO_Port_Index)MyCLI_UnsignedCommandArgument(0);
	GPIO_Pin_Index Pin = (GPIO_Pin_Index)MyCLI_UnsignedCommandArgument(1);

	MyGPIO_Conf2Input(Port,Pin);
}

void MyGPIO_CLI_Set2OutputMode()
{
	GPIO_Port_Index Port = (GPIO_Port_Index)MyCLI_UnsignedCommandArgument(0);
	GPIO_Pin_Index Pin = (GPIO_Pin_Index)MyCLI_UnsignedCommandArgument(1);

	MyGPIO_Conf2Output(Port,Pin);
}

#endif
