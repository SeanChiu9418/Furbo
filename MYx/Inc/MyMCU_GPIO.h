/*
 * MyMCU_GPIO.h
 *
 *  Created on: Jul 6, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MYMCU_GPIO_H_
#define INC_MYMCU_GPIO_H_

#include "My_Config.h"

typedef enum
{
	GPIO_PA = 0,
	GPIO_PB,
	GPIO_PC,
	GPIO_PD,
	GPIO_Num_Port
}GPIO_Port_Index;

typedef enum
{
	GPIO_Pin0 = 0,
	GPIO_Pin1,
	GPIO_Pin2,
	GPIO_Pin3,
	GPIO_Pin4,
	GPIO_Pin5,
	GPIO_Pin6,
	GPIO_Pin7,
	GPIO_Pin8,
	GPIO_Pin9,
	GPIO_Pin10,
	GPIO_Pin11,
	GPIO_Pin12,
	GPIO_Pin13,
	GPIO_Pin14,
	GPIO_Pin15,
	GPIO_PinALL,
	GPIO_Num_Pin,

}GPIO_Pin_Index;

typedef enum
{
  GPIOPIN_RESET = 0,
  GPIOPIN_SET
}GPIOPin_State;
	
/* ########################## Device Pin Define List ############################## */

// Port/Group_A
#define SPI_CSN_Port 	GPIO_PA
#define SPI_CSN_Pin 	GPIO_Pin1
#define My_Test3_Port 	GPIO_PA
#define My_Test3_Pin 	GPIO_Pin2
#define LED_G_Port 		GPIO_PA
#define LED_G_Pin 		GPIO_Pin5
#define S2LP_SDN_Port 	GPIO_PA
#define S2LP_SDN_Pin 	GPIO_Pin8

// Port/Group_B
#define My_Test1_Port	GPIO_PB
#define My_Test1_Pin 	GPIO_Pin3
#define PIR_SI_Port 	GPIO_PB
#define PIR_SI_Pin 		GPIO_Pin6
#define PIR_WU_Port 	GPIO_PB	
#define PIR_WU_Pin 		GPIO_Pin7

// Port/Group_C
#define My_Test2_Port	GPIO_PC		
#define My_Test2_Pin 	GPIO_Pin0
#define STPMT_IN1_Port 	GPIO_PC
#define STPMT_IN1_Pin	GPIO_Pin0
#define STPMT_IN2_Port 	GPIO_PC
#define STPMT_IN2_Pin	GPIO_Pin1
#define STPMT_IN3_Port 	GPIO_PC
#define STPMT_IN3_Pin	GPIO_Pin2
#define STPMT_IN4_Port 	GPIO_PC
#define STPMT_IN4_Pin	GPIO_Pin3
#define Button_B1_Port 	GPIO_PC
#define Button_B1_Pin	GPIO_Pin13

// Port/Group_D


// GPIO Port/Pin Declaration
#ifdef MCU_STM32	

extern GPIO_TypeDef *GPIO_Port[GPIO_Num_Port];
extern uint16_t GPIO_Pin[GPIO_Num_Pin];

#elif defined(MCU_Nuvoton)

extern GPIO_T *GPIO_Port[GPIO_Num_Port];
extern uint16_t GPIO_Pin[GPIO_Num_Pin];

#endif


// GPIO ENABLE/DISABLE
#ifdef MCU_STM32

#define MyGPIO_PortA_Enable	__HAL_RCC_GPIOA_CLK_ENABLE
#define MyGPIO_PortB_Enable	__HAL_RCC_GPIOB_CLK_ENABLE
#define MyGPIO_PortC_Enable	__HAL_RCC_GPIOC_CLK_ENABLE
#define MyGPIO_PortD_Enable	__HAL_RCC_GPIOD_CLK_ENABLE

#elif defined(MCU_Nuvoton)
#endif	

// GPIO Read/Write
#ifdef MCU_STM32

#define MyGPIO_PinBitSet(A,B)	(A)->BSRR = (uint32_t)(B)	// without Checking the parameters For Driver developing
#define MyGPIO_PinBitReset(A,B)	(A)->BRR = (uint32_t)(B)	// without Checking the parameters For Driver developing
#define MyGPIO_PinInputValue(A,B) (A)->IDR & (B)			// without Checking the parameters For Driver developing

#elif defined(MCU_Nuvoton)

#if 0
#define MyGPIO_PinBitSet(A,B)		( GPIO_PIN_DATA((A),(B)) = 1 )
#define MyGPIO_PinBitReset(A,B)		( GPIO_PIN_DATA((A),(B)) = 0 )
#define MyGPIO_PinInputValue(A,B)	( GPIO_PIN_DATA((A),(B)) )
#else

#define MyGPIO_PinBitSet(A,B)		do{												\
										GPIO_Port[(A)]->DATMSK = ~GPIO_Pin[(B)];	\
										GPIO_Port[(A)]->DOUT = GPIO_Pin[(B)];		\
									}while(0)										\

#define MyGPIO_PinBitReset(A,B)		do{												\
										GPIO_Port[(A)]->DATMSK = ~GPIO_Pin[(B)];	\
										GPIO_Port[(A)]->DOUT = ~GPIO_Pin[(B)];		\
									}while(0)										\

#define MyGPIO_PinInputValue(A,B) (GPIO_Port[(A)]->PIN & GPIO_Pin[(B)])	

#endif
//#define MyGPIO_WritePin(A,B,C) 	HAL_GPIO_WritePin((A), (B), (C))
//#define MyGPIO_ReadPin(A,B) 	HAL_GPIO_ReadPin((A),(B))
#endif	

									
// 
#ifdef Dev_PIR_1598
#define MyGPIO_SetPinModeInput_PIR(A,B)		do{												\
											uint32_t temp;									\
											temp = (A)->MODER;								\
											temp &= ~(0xc000);								\
											temp |= (0x0000);								\
											(A)->MODER = temp;								 \
											}while(0)

#define MyGPIO_SetPinModeOUtput_PIR(A,B)	do{												\
												uint32_t temp;								\
												temp = (A)->MODER;							\
												temp &= ~(0xc000);							\
												temp |= ((0x4000));							\
												(A)->MODER = temp;							\
											}while(0)
#endif											

#ifdef MCU_STM32
#define MyGPIO_SetPinModeInput(A,B)		do{												\
											int position = 0;							\
											while (((B) >> position) != 0x00u)			\
											{											\
	  	  	  	  	  	  	  	  	  	  	 	 if(((B) >> position) == 0x01u)			\
												 	 break;								\
												 position++;							\
											}											\
											uint32_t temp;								\
											temp = (A)->MODER;							\
											temp &= ~(GPIO_MODER_MODE0 << ((position) * 2u));\
											temp |= ((GPIO_MODE_INPUT & 0x00000003u) << ((position) * 2u));\
											(A)->MODER = temp;							\
										}while(0)

#define MyGPIO_SetPinModeOUtput(A,B)	do{												\
											int position = 0;							\
											while (((B) >> position) != 0x00u)			\
											{											\
												if(((B) >> position) == 0x01u)			\
													break;								\
												position++;							\
											}											\
											uint32_t temp;								\
											temp = (A)->MODER;							\
											temp &= ~(GPIO_MODER_MODE0 << ((position) * 2u));\
											temp |= ((GPIO_MODE_OUTPUT_PP & 0x00000003u) << ((position) * 2u));\
											(A)->MODER = temp;							\
										}while(0)


// Config GPIO to RISING interrupt
#define MyGPIO_Conf2IT_RISING(A,B) 		do { 											\
											GPIO_InitTypeDef GPIO_InitStruct = {0};		\
											GPIO_InitStruct.Pin = B;					\
											GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;	\
											GPIO_InitStruct.Pull = GPIO_NOPULL;			\
											HAL_GPIO_Init(A, &GPIO_InitStruct);			\
										} while(0)

#elif defined(MCU_Nuvoton)

// Config GPIO to RISING interrupt
#define MyGPIO_Conf2IT_RISING(A,B) 		GPIO_EnableInt(PB, 4, GPIO_INT_RISING);
#endif

void MyGPIO_Init();
void MyGPIO_Conf2Output(GPIO_Port_Index Port, GPIO_Pin_Index Pin);
void MyGPIO_Conf2Input(GPIO_Port_Index Port, GPIO_Pin_Index Pin);

#ifdef MyFunc_CLI
										
#define GPIO_COMMAND_TABLE \
{ "IO_Init",	MyGPIO_Init, "", " GPIO INIT" },  \
{ "IO_W",      	MyGPIO_CLI_WritePin, "uuu", " GPIO_Write ex: \"IO_W Port Pin Status\"\r\n GPIO Port Number ex: 0:PortA ~ 3:PortD \r\n GPIO Pin Number 0:Pin0 ~ 15:Pin15 \r\n GPIO Pin Status Set 0:Low 1:High" },\
{ "IO_R",      	MyGPIO_CLI_ReadPin, "uu", " GPIO_Read ex: \"IO_R Port Pin \"\r\n GPIO Port Number ex: 0:PortA ~ 3:PortD \r\n GPIO Pin Number 0:Pin0 ~ 15:Pin15 "},\
{ "IO_S2I",		MyGPIO_CLI_Set2InputMode, "uu", " GPIO_Set2InputMode ex: \"IO_S2I Port Pin \"\r\n GPIO Port Number ex: 0:PortA ~ 3:PortD \r\n GPIO Pin Number 0:Pin0 ~ 15:Pin15 "},\
{ "IO_S2O",		MyGPIO_CLI_Set2OutputMode, "uu", " GPIO_Set2OutputMode ex: \"IO_S2O Port Pin \"\r\n GPIO Port Number ex: 0:PortA ~ 3:PortD \r\n GPIO Pin Number 0:Pin0 ~ 15:Pin15 "},\
{ "Dummy",   MyGPIO_Init, "", "" }

void MyGPIO_Init();
void MyGPIO_CLI_WritePin();
void MyGPIO_CLI_ReadPin();
void MyGPIO_CLI_Set2InputMode();
void MyGPIO_CLI_Set2OutputMode();
#endif

#endif /* INC_MYMCU_GPIO_H_ */
