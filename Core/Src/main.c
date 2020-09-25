/**************************************************************************//**
 * @file     main.c
 * @version  V0.10
 * @brief    A project template for M251 MCU.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
 ****************************************************************************/

#include <stdio.h>
#include "NuMicro.h"
#include "My_Config.h"
#define RXBUFSIZE   1024

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t g_au8RecData[RXBUFSIZE]  = {0};

volatile uint32_t g_u32ComRbytes = 0;
volatile uint32_t g_u32ComRhead  = 0;
volatile uint32_t g_u32ComRtail  = 0;
volatile int32_t g_i32Wait         = TRUE;


void UART_TEST_HANDLE(void);
void UART_FunctionTest(void);
void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable Internal RC 12MHz clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Waiting for Internal RC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Enable UART clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select UART clock source from HIRC */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    Uart0DefaultMPF();

    /* Lock protected registers */
    SYS_LockReg();
}




/*
 * This is a template project for M251 series MCU. Users could based on this project to create their
 * own application without worry about the IAR/Keil project settings.
 *
 * This template application uses external crystal as HCLK source and configures UART0 to print out
 * "Hello World", users may need to do extra system configuration based on their system design.
 */
int main()
{
	int i,j;
    SYS_Init();

    /* Init UART to 115200-8n1 for print message */
    UART_Open(UART0, 115200);
	MyUart_Init();
	MyGPIO_Init();
	MyMoto_Init();
	printf("\nStart Main loop\n");
    while (1)
	{
		if(MyCLI_ProcessCmdInput())
		{
			//if (interactive)
			//{
				printf(">");
			//}
		}		
		//printf("\nInput: ");
		//MyGPIO_PinBitSet(My_Test1_Port,My_Test1_Pin);	
		//My_Delay(1000);
/*
		My_Delay(1);
		if (MyGPIO_PinInputValue(My_Test3_Port,My_Test3_Pin))
		{
			MyGPIO_PinBitSet(My_Test2_Port,My_Test2_Pin);
		}
		else
		{
			MyGPIO_PinBitReset(My_Test2_Port,My_Test2_Pin);
		}
		My_Delay(500);
		MyGPIO_PinBitReset(My_Test1_Port,My_Test1_Pin);	
		My_Delay(1);
		if (MyGPIO_PinInputValue(My_Test3_Port,My_Test3_Pin))
		{
			MyGPIO_PinBitSet(My_Test2_Port,My_Test2_Pin);
		}
		else
		{
			MyGPIO_PinBitReset(My_Test2_Port,My_Test2_Pin);
		}
		
		My_Delay(500);	
	*/
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle UART Channel 0 interrupt event                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void UART0_IRQHandler(void)
{
	MyUart_IRQHandler();
}



