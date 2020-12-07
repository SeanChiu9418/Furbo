/*
 * MyMCU_SPI.h
 *
 *  Created on: Aug 6, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MYMCU_SPI_H_
#define INC_MYMCU_SPI_H_

#include "My_Config.h"


#define SdkEvalSPICSLow()        MyGPIO_WritePin(SPI_CSN_Port, SPI_CSN_Pin, GPIO_PIN_RESET)
#define SdkEvalSPICSHigh()       MyGPIO_WritePin(SPI_CSN_Port, SPI_CSN_Pin, GPIO_PIN_SET)

void MySPI_TransmitReceive_DMA(uint8_t *pTxData, uint8_t *pRxData,uint16_t Size);
#endif /* INC_MYMCU_SPI_H_ */
