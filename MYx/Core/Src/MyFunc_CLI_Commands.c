/*
 * MyFunc_CLI_Commands.c
 *
 *  Created on: Aug 6, 2020
 *      Author: SeanChiu
 */
#include <stdio.h>
#include <string.h>
#include "My_Config.h"
#include "MyFunc_CLI.h"
#include "MyFunc_CLI_Commands.h"

//extern void helpAction(void);

CommandEntry CommandTable[] = {
  { "help", helpAction, "", "List commands"},
  { "reboot", MyCLI_RebootAction, "", "Reboot device"},
  //{ "dfu", dfuAction, "", "put the device in dfu mode (cli disabled)"},
  //{ "interactive", interactiveAction, "u", "Set interactive mode"},
  //{ "fwVersion", fwVersionAction, "", "Get fw version"},
  
 #ifdef MCU_GPIO 
  GPIO_COMMAND_TABLE,
#endif
  
#ifdef Dev_SubG_S2LP     
  S2LP_COMMAND_TABLE, 
#endif  
 
#ifdef Dev_PIR_1598  
  PIR_COMMAND_TABLE,
#endif

#ifdef Dev_Motor  
  Motor_CLI_TABLE,
#endif
  
  { NULL, NULL, NULL, NULL } // NULL action makes this a terminator
};

void helpAction(void)
{
  CommandEntry *cmd;

  for (cmd = CommandTable; cmd->name != NULL; cmd++) {
    if(!CMD_HIDDEN(cmd))
    {
      printf ("%s %s %s\r\n\r\n", cmd->name, cmd->argumentTypes, cmd->description);
    }
  }
}

void MyCLI_RebootAction(void)
{
  My_printf("{&N API call...&ts} reboot");
	
#ifdef	MCU_STM32
	
  HAL_NVIC_SystemReset();
	
#elif defined(MCU_Nuvoton)
	
	SYS_ResetChip();

#endif	
	
}

