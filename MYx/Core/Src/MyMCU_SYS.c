/*
 * MyMCU_SYS.c
 *
 *  Created on: Jul 8, 2020
 *      Author: SeanChiu
 */
 #include <stdio.h>
#include "MyMCU_SYS.h"

int SysClock;

void MySYS_GetSYSClock()
{

#ifdef MCU_STM32
	
	SysClock = (long)HAL_RCC_GetSysClockFreq();
	
#elif defined(MCU_Nuvoton)
	
	SysClock = SystemCoreClock;
	
#endif
	

}

void My_usDelay(int us)	// 0 ==> 66us, 1 ==> 73us, 5 ==> 100us
{
	//long SySFeq = MySYS_GetSYSClock();
	int Cnt = us * (int)(SysClock/1000000),i;
	
	for( i = 0; i < Cnt; i++)	
#ifdef MCU_STM32		
		asm("nop");
#elif defined(MCU_Nuvoton)	
		__NOP();	
#endif		
}

void My_SysTickInit()
{
#ifdef MCU_STM32
	// Please refer to Below STM32 Function "HAL_InitTick" & Implement
#elif defined(MCU_Nuvoton)
	
#endif
}

#ifdef MCU_STM32

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  HAL_StatusTypeDef  status = HAL_OK;

  if (uwTickFreq != 0U)
  {
    /*Configure the SysTick to have interrupt in 1ms time basis*/
    if (HAL_SYSTICK_Config(SystemCoreClock / (1000U /uwTickFreq)) == 0U)
    {
      /* Configure the SysTick IRQ priority */
      if (TickPriority < (1UL << __NVIC_PRIO_BITS))
      {
        HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority, 0U);
        uwTickPrio = TickPriority;
      }
      else
      {
        status = HAL_ERROR;
      }
    }
    else
    {
      status = HAL_ERROR;
    }
  }
  else
  {
    status = HAL_ERROR;
  }

  /* Return function status */
  return status;
}
#endif

