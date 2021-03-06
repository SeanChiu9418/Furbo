/*
 * MyMCU_GPIO.c
 *
 *  Created on: Jul 6, 2020
 *      Author: SeanChiu
 */
#include <stdio.h>
#include "MyMCU_GPIO.h"

#ifdef MCU_STM32	

	GPIO_TypeDef *GPIO_Port[GPIO_Num_Port] = {GPIOA,GPIOB,GPIOC,GPIOD};
	uint16_t GPIO_Pin[GPIO_Num_Pin] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5,
									GPIO_PIN_6,	GPIO_PIN_7,	GPIO_PIN_8,	GPIO_PIN_9,GPIO_PIN_10,GPIO_PIN_11,
									GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_14,GPIO_PIN_15,GPIO_PIN_All,};
#elif defined(MCU_Nuvoton)

	GPIO_T *GPIO_Port[GPIO_Num_Port] = {PA,PB,PC,PD};
	uint32_t GPIO_Pin[GPIO_Num_Pin] = {BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7, 
									BIT8, BIT9, BIT10, BIT11, BIT12, BIT13, BIT14, BIT15,};	
	uint32_t GPIO_Type[INT_Num_Type] = {GPIO_INT_RISING, GPIO_INT_FALLING, GPIO_INT_BOTH_EDGE, GPIO_INT_HIGH, GPIO_INT_LOW};
#endif	

struct GPIO_INT_DEF GPIO_INT[GPIO_Num_INT];

void MyGPIO_Init()
{
	
#ifdef MCU_STM32	

	/* GPIO Ports Clock Enable */
	  MyGPIO_PortA_Enable();
	  MyGPIO_PortB_Enable();
	  MyGPIO_PortC_Enable();
	  //MyGPIO_PortD_Enable();

	  /*Configure GPIO pin Output Level */
	  // GPIOA, GPIO_PIN_15 -- WIFI_Enable Pin
	  // GPIOB, GPIO_PIN_3 -- My_Test1_Pin
	  // GPIOB, GPIO_PIN_9 -- MIC_Switch Pin
	  // GPIOC, GPIO_PIN_10 -- My_Test2_Pin

	  MyGPIO_WritePin(WIFI_ENB_Port, WIFI_ENB_Pin,GPIO_PIN_RESET);
	  MyGPIO_WritePin(MIC_SW_Port, MIC_SW_Pin,GPIO_PIN_RESET);
	  MyGPIO_WritePin(My_Test1_Port, My_Test1_Pin,GPIO_PIN_RESET);
	  MyGPIO_WritePin(LED_G_Port, LED_G_Pin,GPIO_PIN_RESET);

	  /*Configure GPIO pin : My_Test1_Pin & My_Test2_Pin & WIFI_Enable_Pin & MIC_Switch_Pin*/
	  MyGPIO_Conf2Output(WIFI_ENB_Port, WIFI_ENB_Pin);
	  MyGPIO_Conf2Output(MIC_SW_Port, MIC_SW_Pin);
	  MyGPIO_Conf2Output(My_Test1_Port, My_Test1_Pin);
	  MyGPIO_Conf2Output(LED_G_Port, LED_G_Pin);

	  /*Configure GPIO pin : Button_B1_Pin */
	  MyGPIO_Conf2Input(Button_B1_Port,Button_B1_Pin);
	  MyGPIO_Conf2Input(My_Test2_Port, My_Test2_Pin);

	  MyGPIO_WritePin(WIFI_ENB_Port, WIFI_ENB_Pin,GPIO_PIN_RESET);
	  MyGPIO_WritePin(MIC_SW_Port, MIC_SW_Pin,GPIO_PIN_SET);
	
#elif defined(MCU_Nuvoton)

	//GPIO_SetMode(PB, BIT3, GPIO_MODE_OUTPUT);
	//GPIO_SetMode(PC, BIT0, GPIO_MODE_OUTPUT);
	MyGPIO_Conf2Output(My_Test1_Port,My_Test1_Pin);
	MyGPIO_Conf2Output(My_Test2_Port,My_Test2_Pin);
	MyGPIO_PinBitReset(My_Test1_Port,My_Test1_Pin);
	MyGPIO_PinBitReset(My_Test2_Port,My_Test2_Pin);
	//MyGPIO_Conf2Input(My_Test3_Port,My_Test3_Pin);

#endif	

}

void MyGPIO_SetINTType(GPIO_INT_Index index,GPIO_INT_Type type)
{
	GPIO_INT[index].Type = type;
}

void MyGPIO_DisableINT(GPIO_INT_Index index)
{
	GPIO_DisableInt(GPIO_Port[GPIO_INT[index].Port], GPIO_INT[index].Pin);
}

void MyGPIO_EnableINT(GPIO_INT_Index index)
{
#ifdef MCU_STM32	
	
#elif defined(MCU_Nuvoton)

    GPIO_SetMode(GPIO_Port[GPIO_INT[index].Port], GPIO_Pin[GPIO_INT[index].Pin], GPIO_MODE_INPUT);
    GPIO_EnableInt(GPIO_Port[GPIO_INT[index].Port], GPIO_INT[index].Pin, GPIO_Type[GPIO_INT[index].Type]);
	
#endif	
}


void MyGPIO_Conf2INT_RISING(GPIO_Port_Index Port, GPIO_Pin_Index Pin)	// Config GPIO in Interrupt Rising Mode
{
#ifdef MCU_STM32	
	
#elif defined(MCU_Nuvoton)

    GPIO_SetMode(GPIO_Port[Port], GPIO_Pin[Pin], GPIO_MODE_INPUT);
    GPIO_EnableInt(GPIO_Port[Port], Pin, GPIO_INT_RISING);
	
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

void MyGPIO_DeBounce()
{
#ifdef MCU_STM32	
	
#elif defined(MCU_Nuvoton)	
	
    /* Enable interrupt de-bounce function and select de-bounce sampling cycle time is 1024 clocks of LIRC clock */
    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCTL_DBCLKSRC_LIRC, GPIO_DBCTL_DBCLKSEL_1);
    GPIO_ENABLE_DEBOUNCE(GPIO_Port[STPMT1_ENC_Port], GPIO_Pin[STPMT1_ENC_Pin]);
    GPIO_ENABLE_DEBOUNCE(GPIO_Port[STPMT2_ENC_Port], GPIO_Pin[STPMT2_ENC_Pin]);
    GPIO_ENABLE_DEBOUNCE(GPIO_Port[DCMT_ENC_Port], GPIO_Pin[DCMT_ENC_Pin]);	
	
#endif	
}
	

#ifdef MyFunc_SelfTest

void MyGPIO_SelfTest()
{
	while(1)
	{

		if (MyGPIO_ReadPin(Button_B1_Port,Button_B1_Pin) != GPIOPIN_RESET)
		{
			MyGPIO_WritePin(My_Test1_Port, My_Test1_Pin,GPIOPIN_SET);
			//My_Delay(1);
			if ((MyGPIO_PinInputValue(My_Test2_Port,My_Test2_Pin)) == GPIOPIN_SET)
				MyGPIO_PinBitSet(LED_G_Port, LED_G_Pin);
			else
				MyGPIO_PinBitReset(LED_G_Port, LED_G_Pin);
			My_Delay(500);

			MyGPIO_WritePin(My_Test1_Port, My_Test1_Pin,GPIOPIN_RESET);
			//My_Delay(1);
			if ((MyGPIO_PinInputValue(My_Test2_Port,My_Test2_Pin)) == GPIOPIN_SET)
				MyGPIO_PinBitSet(LED_G_Port, LED_G_Pin);
			else
				MyGPIO_PinBitReset(LED_G_Port, LED_G_Pin);
			My_Delay(500);
		}
	}
}

#endif

void MyGPIO_IRQHandler()
{
	//printf("UART0->INTSTS = %d \n",UART0->INTSTS);
#ifdef MCU_STM32
#elif defined(MCU_Nuvoton)	



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

	printf("GPIO Port:%d Pin:%d = %d",Port,Pin,result);
	//My_printf(int2str(result));

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
