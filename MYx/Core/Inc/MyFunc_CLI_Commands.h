/*
 * MyFunc_CLI_Commands.h
 *
 *  Created on: Jul 29, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MYFUNC_CLI_COMMANDS_H_
#define INC_MYFUNC_CLI_COMMANDS_H_


#include "My_Config.h"
#include "MyDev_Inc.h"

extern CommandEntry CommandTable[];

#define CMD_HIDDEN(CMD)	(strcmp(CMD->name,"EepromWritePage")==0 || \
		strcmp(CMD->name,"EepromReadPage")==0 || strcmp(CMD->name,"EepromStatus")==0 || \
		strcmp(CMD->name,"interactive")==0 || \
		strcmp(CMD->name,"dfu")==0 || \
		strcmp(CMD->name,"RegulateVRf")==0 || \
		strcmp(CMD->name,"GetVRf")==0)

//uint32_t MyCLI_UnsignedCommandArgument(uint8_t argNum);
void MyCLI_RebootAction(void);
void helpAction(void);
void FW_Version(void);
#endif /* INC_MYFUNC_CLI_COMMANDS_H_ */
