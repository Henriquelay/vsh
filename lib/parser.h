/**
* This library is for parsing commandlines strings and treating user input (not signals)
**/

#ifndef _VSH_PARSER_H_
#define _VSH_PARSER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Declared in assignment's specification
#define MAX_COMMANDS (5)
#define MAX_ARGUMENTS (3)
#define COMMAND_DELIM (" | ")
#define ARGS_DELIM (" ")

#define BUFFERSIZE (1000)

// Defines a whole command or shell instruction, including arguments
typedef struct command_t {
    char* commandName;
    char* argument[MAX_ARGUMENTS];
    // The number of arguments used
    unsigned int argc;
} command_t;

// Defines a whole shell input line
typedef struct commandLine_t {
    command_t* command[MAX_COMMANDS];
    // Number of commands used
    unsigned int commandc;
} commandLine_t;

#endif
