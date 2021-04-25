/**
* This library is for executing commands
**/

#include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../lib/parser.h"


#ifndef _VSH_EXECUTERS_H_
#define _VSH_EXECUTERS_H_

// int execSingle(command_t *command);

// int execPiped(commandLine_t *commands);

// int execBultin(command_t *command);

int execCommandLine(commandLine_t *commands);

// int commandCheck(command_t *command);

#endif
