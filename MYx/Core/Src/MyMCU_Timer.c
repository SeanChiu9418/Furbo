/*
 * MyMCU_Timer.c
 *
 *  Created on: Sep 30, 2020
 *      Author: SeanChiu
 */

#include <stdio.h>
#include "MyMCU_Timer.h"

#ifdef MCU_STM32
#elif defined(MCU_Nuvoton)

	TIMER_T *Timer_Port[Timer_Num] = 	{TIMER0, TIMER1, TIMER2, TIMER3};
	uint32_t Timer_Module[Timer_Num] = 	{TMR0_MODULE, TMR1_MODULE, TMR2_MODULE, TMR3_MODULE};
	IRQn_Type Timer_IRQn[Timer_Num] = 	{TMR0_IRQn, TMR1_IRQn, TMR2_IRQn, TMR3_IRQn};
	uint32_t Timer_ClkSrc[Timer_Num] = 	{CLK_CLKSEL1_TMR0SEL_PCLK0, CLK_CLKSEL1_TMR1SEL_PCLK0, CLK_CLKSEL1_TMR2SEL_PCLK1, CLK_CLKSEL1_TMR3SEL_PCLK1};
	
#endif	

	
void MyTimer0_IRQHandler()
{
	printf("MyTimer0_IRQHandler\r\n");
    // clear timer interrupt flag	
    TIMER_ClearIntFlag(TIMER0);	
}
	
void MyTimer1_IRQHandler()
{
	//printf("MyTimer1_IRQHandler\r\n");
    // clear timer interrupt flag	
	MyMTENC_NVIC_DCTimeOut();
    TIMER_ClearIntFlag(TIMER1);	
}

void MyTimer_EnableInt(Timer_Index Port)
{
    TIMER_EnableInt(Timer_Port[Port]);
    //NVIC_EnableIRQ(Timer_IRQn[Port]);
    MyNVIC_EnableIRQ(Timer_IRQn[Port]);	
}

void MyTimer_Init(Timer_Index Port, Timer_Mode Mode,uint32_t u32Freq)
{
 switch(Mode)
 {
	 case Counter_Mode:
#ifdef MCU_STM32	
#elif defined(MCU_Nuvoton)
	 
	CLK_EnableModuleClock(Timer_Module[Port]);
	CLK_SetModuleClock(Timer_Module[Port], Timer_ClkSrc[Port], 0);
	 
    if (TIMER_Open(Timer_Port[Port], TIMER_PERIODIC_MODE, u32Freq) != 1)
    {
        printf("Set the frequency different from the user\n");
    }	 
	
#endif		 
	 
		 break;
	 case PWM_Mode:
		 break;
	 case Timer_Mode_Num:
		 break;
 }
	 
}
	
#ifdef MyFunc_SelfTest
void MyTimer_TestInit(Timer_Index Index)
{

    // Set timer frequency to 1HZ
    if (TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 1) != 1)
    {
        printf("Set the frequency different from the user\n");
    }

    // Enable timer interrupt
    TIMER_EnableInt(TIMER0);
    NVIC_EnableIRQ(TMR0_IRQn);

    // Start Timer 0
    TIMER_Start(TIMER0);	
	
}
#endif
