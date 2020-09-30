/*
 * MyFunc_Tossing.c
 *
 *  Created on: Sep 28, 2020
 *      Author: SeanChiu
 */

#include <stdio.h>
#include "MyFunc_Tossing.h"

void MyTossing_Init()
{
	MyMoto_Init();

    /*-----------------------------------------------------------------------------------------------------*/
    /* GPIO Interrupt Function Test                                                                        */
    /*-----------------------------------------------------------------------------------------------------*/

    /* Configure PB.5 as Input mode and enable interrupt by rising edge trigger */
	MyGPIO_Conf2INT_RISING(STPMT1_ENC_Port,STPMT1_ENC_Pin);

    /* Configure PB.4 as Input mode and enable interrupt by rising edge trigger */
	MyGPIO_Conf2INT_RISING(DCMT_ENC_Port,DCMT_ENC_Pin);
	
    /* Configure PB.2 as Input mode and enable interrupt by rising edge trigger */
	MyGPIO_Conf2INT_RISING(STPMT2_ENC_Port,STPMT2_ENC_Pin);

	MyNVIC_EnableIRQ(GPB_IRQn);
	
	/* Enable interrupt de-bounce function*/
	MyGPIO_DeBounce();
	
}
