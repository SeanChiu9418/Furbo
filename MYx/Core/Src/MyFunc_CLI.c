/*
 * MyFunc_CLI.c
 *
 *  Created on: Jul 10, 2020
 *      Author: SeanChiu
 */
#include <stdio.h>
#include <string.h>
#include "MyFunc_CLI.h"
#include "MyFunc_CLI_Commands.h"

//#include "stm32g0xx_hal.h"

static volatile uint8_t interactive = 1;
static CMDStatus CMD_Status = CMD_Null;
static char CMD[COMMAND_BUFFER_LENGTH] = {0}     ;
static CommandEntry* extCommandFinger=NULL;

//------------------------------------------------------------------------------
// Command parsing state

typedef struct {

  // Finite-state machine's current state.
  uint8_t state;

  // The command line is stored in this buffer.
  // Spaces and trailing '"' and '}' characters are removed,
  // and hex strings are converted to bytes.
  uint8_t buffer[COMMAND_BUFFER_LENGTH];

  // Indices of the tokens (command(s) and arguments) in the above buffer.
  // The (+ 1) lets us store the ending index.
  uint8_t tokenIndices[MAX_TOKEN_COUNT + 1];

  // The number of tokens read in, including the command(s).
  uint8_t tokenCount;

  // Used while reading in the command line.
  uint16_t index;

  // First error found in this command.
  uint8_t error;

  // Storage for reading in a hex string. A value of 0xFF means unused.
  uint8_t hexHighNibble;

  // The token number of the first true argument after possible nested commands.
  uint8_t argOffset;

} CommandState;

CommandState commandState;


static uint8_t MyCLI_FirstByteOfArg(uint8_t argNum)
{
  uint8_t tokenNum = argNum + commandState.argOffset;
  return commandState.buffer[commandState.tokenIndices[tokenNum]];
}

uint8_t MyCLI_tokenLength(uint8_t num)
{
  return (commandState.tokenIndices[num + 1]
          - commandState.tokenIndices[num]);
}

uint8_t *MyCLI_tokenPointer(int8_t tokenNum)
{
  return commandState.buffer + commandState.tokenIndices[tokenNum];
}


const char * CommandErrorNames[] =
  {
    "no error",
    "serial port error",
    "no such command",
    "wrong number of arguments",
    "integer argument out of range",
    "argument syntax error",
    "string too long",
    "invalid argument type"
  };
void MyCLI_CommandErrorHandler(CommandStatus status)
{
	//My_printf((char *)CommandErrorNames[status]);
    printf("%s\r\n", CommandErrorNames[status]);
}

void MyCLI_CommandReaderInit(void)
{
  commandState.state = CMD_AWAITING_ARGUMENT;
  commandState.index = 0;
  commandState.tokenIndices[0] = 0;
  commandState.tokenCount = 0;
  commandState.error = CMD_SUCCESS;
  commandState.hexHighNibble = 0xFF;
}

static CommandEntry *MyCLI_CommandLookup(CommandEntry *commandFinger,
                                        uint8_t tokenNum)
{
  uint8_t *command = MyCLI_tokenPointer(tokenNum);
  uint8_t length = MyCLI_tokenLength(tokenNum);
  uint8_t i;
  for (i = 0;
       commandFinger->action != NULL;
       i++, commandFinger++) {
    const char * name = commandFinger->name;
    uint8_t *finger = command;
    for (;; name++, finger++) {
      if (finger - command == length) {
        if (*name == 0) {
          return commandFinger;
        } else {
          break;
        }
      } else if (My_CharDowncase(*finger) != My_CharDowncase(*name)) {
        break;
      }
    }
  }
  return NULL;
}


static uint32_t MyCLI_StringToUnsignedInt(uint8_t argNum, uint8_t swallowLeadingSign)
{
  uint8_t tokenNum = argNum + commandState.argOffset;
  uint8_t *string = commandState.buffer + commandState.tokenIndices[tokenNum];
  uint8_t length = MyCLI_tokenLength(tokenNum);
  uint32_t result = 0;
  uint8_t base = 10;
  uint8_t i;
  for (i = 0; i < length; i++)
	{
    uint8_t next = string[i];
    if (swallowLeadingSign && i == 0 && next == '-')
		{
      // do nothing
    }
		else if ((next == 'x' || next == 'X')	&& result == 0)
		{
      base = 16;
    }
		else
		{
      uint8_t value = My_HexToInt(next);
      if (value < base)
			{
        result = result * base + value;
      }
			else
			{
        commandState.error = CMD_ERR_ARGUMENT_SYNTAX_ERROR;
        return 0;
      }
    }
  }
  return result;
}

uint32_t MyCLI_UnsignedCommandArgument(uint8_t argNum)
{
  return MyCLI_StringToUnsignedInt(argNum, false);
}

static void MyCLI_CallCommandAction(void)
{
  CommandEntry *commandFinger = CommandTable;
  uint8_t tokenNum = 0;
  uint8_t i;

  if (commandState.tokenCount == 0) {
    goto kickout2;
  }

  // Lookup the command.
  while (true) {
    commandFinger = MyCLI_CommandLookup(commandFinger, tokenNum);
    if (commandFinger != NULL)
    {
      tokenNum += 1;
      if (commandFinger->argumentTypes[0] == 'n')
      {
        // Nested commands are implemented by overloading the action
        // field of a command with a pointer to another table of
        // commands.
        commandFinger = (CommandEntry *)(void *)(commandFinger->action);
      }
      else
      {
        commandState.argOffset = tokenNum;
        break;
      }
    }
    else
    {
      commandState.error = CMD_ERR_NO_SUCH_COMMAND;
      goto kickout;
    }
  }

  // Validate the arguments.
  for(i = 0; tokenNum < commandState.tokenCount; tokenNum++, i++) {
    uint8_t type = commandFinger->argumentTypes[i];
    uint8_t firstChar = MyCLI_FirstByteOfArg(i);
    switch(type)
    {

    // Integers
    case 'u':
    case 'v':
    case 'w':
    case 's':
    {
      uint32_t limit = (type == 'u' ? 0xFF
                      : (type == 'v' ? 0xFFFF
                         : (type =='s' ? 0x7F : -1)));
      if (MyCLI_StringToUnsignedInt(i, true) > limit)
      {
        commandState.error = CMD_ERR_ARGUMENT_OUT_OF_RANGE;
      }
      break;
    }

    // String
    case 'b':
      if (firstChar != '"' && firstChar != '{')
      {
        commandState.error = CMD_ERR_ARGUMENT_SYNTAX_ERROR;
      }
      break;

    case 0:
      commandState.error = CMD_ERR_WRONG_NUMBER_OF_ARGUMENTS;
      break;

    default:
      commandState.error = CMD_ERR_INVALID_ARGUMENT_TYPE;
      break;
    }

    if (commandState.error != CMD_SUCCESS) {
      goto kickout;
    }
  }

  if (0 != commandFinger->argumentTypes[commandState.tokenCount
                                        - commandState.argOffset]) {
    commandState.error = CMD_ERR_WRONG_NUMBER_OF_ARGUMENTS;
  }

 kickout:

  if (commandState.error == CMD_SUCCESS) {
    commandFinger->action();
    extCommandFinger = commandFinger;
  } else {
	  MyCLI_CommandErrorHandler(commandState.error);
  }

 kickout2:

 MyCLI_CommandReaderInit();
}


//----------------------------------------------------------------
// Retrieving arguments


//----------------------------------------------------------------
// This is a state machine for parsing commands.  If 'input' is NULL
// 'sizeOrPort' is treated as a port and characters are read from there.
//
// Goto's are used where one parse state naturally falls into another,
// and to save flash.

uint8_t MyCLI_ProcessCommandString(uint8_t *input, uint8_t sizeOrPort)
{
  uint8_t isEol = false;
  uint8_t isSpace, isQuote;

  while (true)
  {
    uint8_t next;

    if (input == NULL)
	{/*
      switch (__io_getcharNonBlocking(&next))
      {
      case TRUE:
        break;
      case FALSE:
        return isEol;
      default:
        commandState.error = CMD_ERR_PORT_PROBLEM;
        goto CMD_READING_TO_EOL;
      }
	*/
	}
    else if (sizeOrPort == 0)
	{
      return isEol;
    }
	else
	{
      next = *input;
      input += 1;
      sizeOrPort -= 1;
    }

    //   fprintf(stderr, "[processing '%c' (%s)]\n", next, stateNames[commandState.state]);

    if (next == '\r')
      continue;

    isEol =   (next == '\n');
    isSpace = (next == ' ');
    isQuote = (next == '"');

    switch (commandState.state)
	{

    case CMD_AWAITING_ARGUMENT:
      if (isEol)
      {
    	  MyCLI_CallCommandAction();
      }
      else if (! isSpace)
      {
        if (isQuote)
        {
          commandState.state = CMD_READING_STRING;
        }
        else if (next == '{')
        {
          commandState.state = CMD_READING_HEX_STRING;
        }
        else
        {
          commandState.state = CMD_READING_ARGUMENT;
        }
        goto WRITE_TO_BUFFER;
      }
      break;

    case CMD_READING_ARGUMENT:
      if (isEol || isSpace)
      {
        goto END_ARGUMENT;
      }
      else
      {
        goto WRITE_TO_BUFFER;
      }

    case CMD_READING_STRING:
      if (isQuote)
			{
        goto END_ARGUMENT;
      }
			else
			{
        goto WRITE_TO_BUFFER;
      }

    case CMD_READING_HEX_STRING:
		{
      uint8_t waitingForLowNibble = (commandState.hexHighNibble != 0xFF);
      if (next == '}')
			{
        if (waitingForLowNibble)
				{
          commandState.error = CMD_ERR_ARGUMENT_SYNTAX_ERROR;
          goto CMD_READING_TO_EOL;
        }
        goto END_ARGUMENT;
      }
			else
			{
        uint8_t value = My_HexToInt(next);
        if (value < 16)
				{
          if (waitingForLowNibble)
					{
            next = (commandState.hexHighNibble << 4) + value;
            commandState.hexHighNibble = 0xFF;
            goto WRITE_TO_BUFFER;
          }
					else
					{
            commandState.hexHighNibble = value;
          }
        }
				else if (! isSpace)
				{
          commandState.error = CMD_ERR_ARGUMENT_SYNTAX_ERROR;
          goto CMD_READING_TO_EOL;
        }
      }
      break;
    }

    CMD_READING_TO_EOL:
      commandState.state = CMD_READING_TO_EOL;

    case CMD_READING_TO_EOL:
      if (isEol)
			{
        if (commandState.error != CMD_SUCCESS)
				{
          MyCLI_CommandErrorHandler(commandState.error);
        }
        MyCLI_CommandReaderInit();
      }
      break;

    END_ARGUMENT:
      if (commandState.tokenCount == MAX_TOKEN_COUNT)
			{
        commandState.error = CMD_ERR_WRONG_NUMBER_OF_ARGUMENTS;
        goto CMD_READING_TO_EOL;
      }
      commandState.tokenCount += 1;
      commandState.tokenIndices[commandState.tokenCount] = commandState.index;
      commandState.state = CMD_AWAITING_ARGUMENT;
      if (isEol)
			{
    	  MyCLI_CallCommandAction();
      }
      break;

    WRITE_TO_BUFFER:
      if (commandState.index == COMMAND_BUFFER_LENGTH)
			{
        commandState.error = CMD_ERR_STRING_TOO_LONG;
        goto CMD_READING_TO_EOL;
      }
      if (commandState.state == CMD_READING_ARGUMENT)
			{
        next = My_CharDowncase(next);
      }
      commandState.buffer[commandState.index] = next;
      commandState.index += 1;
      break;
    }
  }
}

//----------------------------------------------------------------
// Command lookup and processing

//uint8_t MyCLI_ProcessCmdInput (uint8_t interactive)	// Deal with CMD
uint8_t MyCLI_ProcessCmdInput ()	// Deal with CMD
{
  static uint8_t buff[COMMAND_BUFFER_LENGTH];
  static uint16_t len;
  //static uint16_t currIndex = 0;
  if (interactive)
  {
    // continue calling SerialReadPartialLine() until it returns success, indicating a full line has been read
    //if (!serialReadPartialLine((char *)buff, COMMAND_BUFFER_LENGTH, &currIndex))
	if (MyUART_GetCMDStatus() != CMD_Received)
	{
      return 0;
    }
	else
		MyUART_GetCMD((char*)buff);

    len=0;
    //search foward looking for first CR, LF, or NULL to determine length of the string
    while((buff[len]!='\n') && (buff[len] !='\r') && (buff[len]!='\0'))
	{
      len++;
    }
    buff[len ++] = '\r'; //set the final char to be CR
    buff[len ++] = '\n'; //set the final char to be NL

    //currIndex = 0;
    return MyCLI_ProcessCommandString(buff, len);
    //printf(buff);

  }
  else
  {
	  /** @brief Process input coming in on the given serial port.
	   * @return TRUE if an end of line character was read.
	   * If the application uses a command line prompt,
	   * this indicates it is time to print the prompt.
	   * @code
	   * void processCommandInput(uint8_t port);
	   * @endcode
	   */
    return MyCLI_ProcessCommandString(NULL,0);
  }
}

void MyCLI_UpdateCMD(char *str)
{
	if (CMD_Status == CMD_Null)
	{
		strcpy(CMD,str);
		//printf(CMD);
		CMD_Status = CMD_Ready;
	}
	else
		printf("@ In Busy");
}
/*
void MyCLI_GetCMD(char *str)
{
	char c[CMD_Len] ="Waiting";
	size_t length = strlen(CMD);
	if (CMD_Status == CMD_Null)
	{
		strcpy(str , c);
	}
	else if (length > CMD_Len)
	{
		printf("@ CMD Too Long");
		CMD_Status = CMD_Null;
	}
	else
		strcpy(str , CMD);
}
*/
void MyCLI_SetStatus(CMDStatus status)
{
	CMD_Status = status;
}
