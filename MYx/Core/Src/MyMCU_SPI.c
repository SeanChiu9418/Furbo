/*
 * MyMCU_SPI.c
 *
 *  Created on: Aug 6, 2020
 *      Author: SeanChiu
 */

#include "MyMCU_SPI.h"

#define SPI_ENTER_CRITICAL()           __disable_irq()
#define SPI_EXIT_CRITICAL()            __enable_irq()

#define DELAY_CS_SCLK			0x70	/* Delay between CSn falling edge & start of SCLK */

//#define SdkEvalSPICSLow()        HAL_GPIO_WritePin(SPI1_CSN_GPIO_Port, SPI1_CSN_Pin, GPIO_PIN_RESET)
//#define SdkEvalSPICSHigh()       HAL_GPIO_WritePin(SPI1_CSN_GPIO_Port, SPI1_CSN_Pin, GPIO_PIN_SET)

#define WAIT_FOR_SPI_TC()               {while(!__HAL_DMA_GET_FLAG(hspi1.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmarx)));\
                                          do{HAL_DMA_IRQHandler(hspi1.hdmarx);\
                                          HAL_DMA_IRQHandler(hspi1.hdmatx);\
                                          }while(HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);}

extern SPI_HandleTypeDef hspi1;

void MySPI_TransmitReceive_DMA(uint8_t *pTxData, uint8_t *pRxData,uint16_t Size)
{
	  SPI_ENTER_CRITICAL();
	  SdkEvalSPICSLow();
	  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);

	  HAL_SPI_TransmitReceive_DMA(&hspi1, pTxData, pRxData, Size);
	  WAIT_FOR_SPI_TC();

	  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
	  SdkEvalSPICSHigh();
	  SPI_EXIT_CRITICAL();
}
