/*
 * MyMCU_PWM.c
 *
 *  Created on: Oct 8, 2020
 *      Author: SeanChiu
 */

#include "MyMCU_PWM.h"

uint32_t PWM_Channel[PMM_Num]={5,4};	// PWM_0 = PWM_CH5 in NUVOTON, PWM_1 = PWM_CH4 in NUVOTON
uint32_t PWM_MASK[PMM_Num] = {PWM_CH_5_MASK,PWM_CH_4_MASK};

void MyPWM_Init()
{
	/* Enable PWM0 module clock */
    CLK_EnableModuleClock(PWM0_MODULE);
	
	/*---------------------------------------------------------------------------------------------------------*/
    /* PWM clock frequency configuration                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* PWM clock frequency can be set equal to HCLK by choosing case 1 */
    /* case 1.PWM clock frequency is set equal to PCLK */
    CLK_SetModuleClock(PWM0_MODULE, CLK_CLKSEL2_PWM0SEL_PCLK0, 0);
    /*---------------------------------------------------------------------------------------------------------*/
	
	/* Reset PWM0 module */
    SYS_ResetModule(PWM0_RST);
	
	/* Set PB multi-function pins for PWM0 Channel 4&5 */
    SYS->GPB_MFPL = (SYS->GPB_MFPL & ~(SYS_GPB_MFPL_PB0MFP_Msk)) | SYS_GPB_MFPL_PB0MFP_PWM0_CH5;
    SYS->GPB_MFPL = (SYS->GPB_MFPL & ~(SYS_GPB_MFPL_PB1MFP_Msk)) | SYS_GPB_MFPL_PB1MFP_PWM0_CH4;
	
}

void MyPWM_Config(PWM_Index CH, uint32_t Freq, uint16_t Duty)
{
	
	PWM_ConfigOutputChannel(PWM0, PWM_Channel[CH], Freq, Duty);
}

void MyPWM_Eanble(PWM_Index CH)		// Nuvoton's PWM Start is Enable 
{
	PWM_Start(PWM0, PWM_MASK[CH]);
}

void MyPWM_Disable(PWM_Index CH)
{	
	PWM_Stop(PWM0, PWM_MASK[CH]);
}

void MyPWM_StartOutput(PWM_Index CH)	// Nuvoton's PWM Enable is Start Output 
{
	PWM_EnableOutput(PWM0, PWM_MASK[CH]);
}

void MyPWM_StopOutput(PWM_Index CH)
{
    PWM_DisableOutput(PWM0, PWM_MASK[CH]);
}

#ifdef MyFunc_CLI

void MyPWM_CLI_Config(void)
{

}

void MyPWM_CLI_Enable(void)
{

}

void MyPWM_CLI_DisEnable(void)
{

}

void MyPWM_CLI_Start(void)
{

}

void MyPWM_CLI_Stop(void)
{

}

#endif