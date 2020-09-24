/*
 * MyMCU_UART.h
 *
 *  Created on: Jul 10, 2020
 *      Author: SeanChiu
 */


#ifndef INC_MYMCU_UART_H_
#define INC_MYMCU_UART_H_

#include "My_Config.h"

#define COMMAND_BUFFER_LENGTH 64

typedef enum
{
	Rx_Clear = 0,
	Rx_Read
}RxRead_Status;

typedef enum
{
	CMD_Clear = 0,
	CMD_Receiving,		// UART RX Receiving
	CMD_Received	// UART CMD Get Ready
}UARTReceive_Status;


/* USER CODE BEGIN 0 */
void MyUart_Init();
void MyUart_IRQHandler();
void MyUart_Receive();
UARTReceive_Status MyUART_GetCMDStatus();
void MyUART_GetCMD(char *str);
void My_printf(char *str);
#endif /* INC_MYMCU_UART_H_ */
