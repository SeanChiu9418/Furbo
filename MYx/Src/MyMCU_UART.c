/*
 * MyMCU_UART.c
 *
 *  Created on: Jul 10, 2020
 *      Author: SeanChiu
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "MyMCU_UART.h"

//static unsigned char uRx_Data;
unsigned char   UART_CMD[COMMAND_BUFFER_LENGTH] = {0};

RxRead_Status Rx_Status = Rx_Clear;
UARTReceive_Status CMD_Status = CMD_Clear;

#ifdef MCU_STM32
extern UART_HandleTypeDef huart2;
#endif

void My_printf(char *str)
{
#ifdef MCU_STM32	
	size_t Size = strlen(str);
	HAL_UART_Transmit(&huart2,(uint8_t*)str,Size,0xffff);
	str = "\r\n";
	HAL_UART_Transmit(&huart2,(uint8_t*)str,2,0xffff);
#elif defined(MCU_Nuvoton)

#endif	
}

void MyUart_Init()
{

#ifdef MCU_STM32	

#elif defined(MCU_Nuvoton)	
	
	/* Unlock protected registers */
    SYS_UnlockReg();

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
    //Uart0DefaultMPF();
    /* Set GPB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~SYS_GPB_MFPH_PB12MFP_Msk) | SYS_GPB_MFPH_PB12MFP_UART0_RXD;
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~SYS_GPB_MFPH_PB13MFP_Msk) | SYS_GPB_MFPH_PB13MFP_UART0_TXD;
	
    /* Lock protected registers */
    SYS_LockReg();

    /* Init UART to 115200-8n1 for print message */
    UART_Open(UART0, 115200);
	
	
    UART_SetTimeoutCnt(UART0, 0x10); // Set Rx Time-out counter

    // Set RX FIFO Interrupt Trigger Level
    UART0->FIFO &= ~ UART_FIFO_RFITL_Msk;
    UART0->FIFO |= UART_FIFO_RFITL_1BYTE;

    /* Enable UART RDA/THRE/Time-out interrupt */
    NVIC_EnableIRQ(UART0_IRQn);
    UART_EnableInt(UART0, (UART_INTEN_RDAIEN_Msk | UART_INTEN_RXTOIEN_Msk));

    /* Disable UART RDA/THRE/Time-out interrupt */
    //UART_DisableInt(UART0, (UART_INTEN_RDAIEN_Msk | UART_INTEN_THREIEN_Msk | UART_INTEN_RXTOIEN_Msk));	

#endif	
}



void MyUart_RxData(uint8_t *pData, uint16_t Size)
{
#ifdef MCU_STM32
		HAL_UART_Receive(&huart2, pRx_Data, Size, 1000);
#elif defined(MCU_Nuvoton)
		*pData = UART_READ(UART0);
#endif		
}

void MyUart_TxData(uint8_t *pData, uint16_t Size)
{
#ifdef MCU_STM32
		HAL_UART_Receive(&huart2, pRx_Data, Size, 1000);
#elif defined(MCU_Nuvoton)
		int i ;
		for (i = 0 ; i <Size; i++)
		{
			while (UART_IS_TX_FULL(UART0)); /* Wait Tx is not full to transmit data */	
			UART_WRITE(UART0, *(pData+i));			
		}	
#endif		
}

void MyUart_Receive()
{

	static unsigned char   uRx_Data[COMMAND_BUFFER_LENGTH] = {0}     ;    //存儲?���?
	static unsigned char * pRx_Data       = uRx_Data;    //??��?��?�儲?��組�?��?��?�儲?��??��?��??
	static unsigned char   uLength        =  0      ;    //?��?��?��??�長�?

	//HAL_UART_Receive(&huart2, pRx_Data, 1, 1000);
	MyUart_RxData(pRx_Data,1);
	
	if ( *pRx_Data > 0x7F)	// handle bogus characters
	{
		return;
	}
	else if( *pRx_Data == 0x8 )	// handle backspace
	{
    	if ( (uLength) > 0 )
    	{
        // delete the last character from our string
    		uLength--;
    		pRx_Data--;
    		uRx_Data[uLength] = '\0';
        // echo backspace
    		unsigned char BK[10]="\010 \010";
    		//HAL_UART_Transmit(&huart2, BK, 3, 0xffff);
			MyUart_TxData(BK,3);

    	}
      // don't add or process this character
	}
	else if((*pRx_Data == '\n') || (*pRx_Data == '\r') ||(uLength >= (COMMAND_BUFFER_LENGTH - 2)))
	{
		*pRx_Data = '\0';
		strcpy((char *)UART_CMD,(char *)uRx_Data);
		CMD_Status = CMD_Received;
		//HAL_UART_Transmit(&huart2, uRx_Data, uLength, 0xffff);
		// New Line
		uRx_Data[0] = '\r';
		uRx_Data[1] = '\n';
		//HAL_UART_Transmit(&huart2, uRx_Data, 2, 0xffff);
		MyUart_TxData(uRx_Data,2);
		pRx_Data = uRx_Data;
		uLength  = 0;

	}else
	{
		//HAL_UART_Transmit(&huart2, pRx_Data, 1, 0xffff);
		MyUart_TxData(pRx_Data,1);
		pRx_Data++;
		uLength++;
		CMD_Status = CMD_Receiving;
	}

}

void MyUart_IRQHandler()
{
	//printf("UART0->INTSTS = %d \n",UART0->INTSTS);
#ifdef MCU_STM32
	if(1)
#elif defined(MCU_Nuvoton)
	if ((UART0->INTSTS & UART_INTSTS_RDAINT_Msk) || (UART0->INTSTS & UART_INTSTS_RXTOINT_Msk))	
#endif
	{	
		MyUart_Receive();
	}

}

UARTReceive_Status MyUART_GetCMDStatus()
{
	return CMD_Status;
}

void MyUART_GetCMD(char *str)
{
	if (CMD_Status == CMD_Received)
	{
		strcpy(str,(char *)UART_CMD);
		//My_printf(CMD);
		CMD_Status = CMD_Clear;
	}
}


