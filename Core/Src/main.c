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
#include "MyMCU_Inc.h"
#include "MyDev_Inc.h"
#include "MyFunc_Inc.h"
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
	MyTossing_Init();
	printf("\nStart Main loop\n");
	
	MyPWM_Config(PWM_0,20000, 50);
	MyPWM_Eanble(PWM_0);
	MyPWM_Start(PWM_0);
	
	
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

/**
 * @brief       GPIO PB IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The PB default IRQ, declared in startup_M251.s.
 */
void GPB_IRQHandler(void)
{
	if (GPIO_GET_INT_FLAG(PB, BIT2))
    {
        GPIO_CLR_INT_FLAG(PB, BIT2);
        printf("PB.2 INT occurred.\n");
    }
    else if(GPIO_GET_INT_FLAG(PB, BIT5))
    {
        GPIO_CLR_INT_FLAG(PB, BIT5);
        printf("PB.5 INT occurred.\n");
    }
    else if(GPIO_GET_INT_FLAG(GPIO_Port[DCMT_ENC_Port], GPIO_Pin[DCMT_ENC_Pin]))
    {		
        printf("PB.4 INT occurred CNT = %d.\r\n", Timer_Port[MTENC[ENC_DCMT]]->CNT);
		Timer_Port[MTENC[ENC_DCMT]]->CNT = 0;
        GPIO_CLR_INT_FLAG(PB, BIT4);		
    }
    else
    {
        /* Un-expected interrupt. Just clear all PB interrupts */
        PB->INTSRC = PB->INTSRC;
        printf("Un-expected interrupts.\n");
    }	
}

void TMR0_IRQHandler(void)
{
	MyTimer0_IRQHandler();
}


void TMR1_IRQHandler(void)
{
	MyTimer1_IRQHandler();
}
