/*
 * MyFunc_CLI.h
 *
 *  Created on: Jul 10, 2020
 *      Author: SeanChiu
 */

#ifndef INC_MYFUNC_CLI_H_
#define INC_MYFUNC_CLI_H_

#include "MyMCU_Inc.h"

/** The maximum number of arguments a command can have.  A nested command
 * counts as an argument.
 */
#define MAX_COMMAND_ARGUMENTS 16
// The (+ 1) takes into account the leading command.
#define MAX_TOKEN_COUNT (MAX_COMMAND_ARGUMENTS + 1)


typedef void (*CommandAction)(void);


typedef struct {
  /** Use letters, digits, and underscores, '_', for the command name.
   * Command names are case-sensitive.
   */
  const char * name;
  /** A reference to a function in the application that implements the
   *  command.
   */
  CommandAction action;
  /** String that specifies the number and types of arguments the
   * command accepts.  The argument specifiers are:
   *  - u:   one-byte unsigned integer.
   *  - v:   two-byte unsigned integer
   *  - w:   four-byte unsigned integer
   *  - s:   one-byte signed integer
   *  - b:   string.  The argument can be entered in ascii by using
   *         quotes, for example: "foo".  Or it may be entered
   *         in hex by using curly braces, for example: { 08 A1 f2 }.
   *         There must be an even number of hex digits, and spaces
   *         are ignored.
   *  - n:   indicates this is a 'n'ested command.
   *         The action points to a table of subcommands.
   *         If used, this must be the only specifier.
   *         It also adds one to the total argument count of the
   *         complete command.
   *
   *  Integer arguments can be either decimal or hexidecimal.
   *  A 0x prefix indicates a hexidecimal integer.  Example: 0x3ed.
   */
  const char * argumentTypes;
  /** A description of the command.
   */
  const char * description;
} CommandEntry;





enum {
  CMD_AWAITING_ARGUMENT,
  CMD_READING_ARGUMENT,
  CMD_READING_STRING,                  // have read opening " but not closing "
  CMD_READING_HEX_STRING,              // have read opening { but not closing }
  CMD_READING_TO_EOL                   // clean up after error
};


typedef uint8_t CommandStatus;
enum

{
  CMD_SUCCESS,
  CMD_ERR_PORT_PROBLEM,
  CMD_ERR_NO_SUCH_COMMAND,
  CMD_ERR_WRONG_NUMBER_OF_ARGUMENTS,
  CMD_ERR_ARGUMENT_OUT_OF_RANGE,
  CMD_ERR_ARGUMENT_SYNTAX_ERROR,
  CMD_ERR_STRING_TOO_LONG,
  CMD_ERR_INVALID_ARGUMENT_TYPE
};


typedef enum
{
	CMD_Null = 0,
	CMD_Ready,
	CMD_Runing
}CMDStatus;

uint32_t MyCLI_UnsignedCommandArgument(uint8_t argNum);
//uint8_t MyCLI_ProcessCmdInput (uint8_t interactive);
uint8_t MyCLI_ProcessCmdInput ();	// Deal with CMD
void MyCLI_UpdateCMD(char *str);
void MyCLI_GetCMD(char *str);
void MyCLI_SetStatus(CMDStatus status);

#endif /* INC_MYFUNC_CLI_H_ */
