/**
* This library is for executing commands
**/

#ifndef _VSH_EXECUTERS_H_
#define _VSH_EXECUTERS_H_

#include "../lib/parser.h"
#include "../lib/signals.h"
#include "../lib/list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int execCommandLine(commandLine_t *commands);

#endif
