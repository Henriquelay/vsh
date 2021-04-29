#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../lib/parser.h"

void freeCommand(command_t *c) {
    c->commandName = NULL;
    for (int i = 0; i <= c->argc; i++) {
        free(c->argument[i]);
        c->argument[i] = NULL;
    }
    free(c);
}

void freeCommandLine(commandLine_t *cl) {
    for (int i = 0; i < cl->commandc; i++) {
        freeCommand(cl->command[i]);
        cl->command[i] = NULL;
    }
    free(cl);
}

command_t *initCommand() {
    command_t *newCommand = (command_t *)malloc(sizeof(command_t));
    if (newCommand == NULL) {
        perror("Error allocating new command_t");
        exit(EXIT_FAILURE);
    }
    newCommand->argc = 0;
    return newCommand;
}

commandLine_t *initCommandLine() {
    commandLine_t *newCommandLine = (commandLine_t *)malloc(sizeof(commandLine_t));
    if (newCommandLine == NULL) {
        perror("Error allocating new commandLine_t");
        exit(EXIT_FAILURE);
    }
    newCommandLine->commandc = 0;
    return newCommandLine;
}

// Handy util to create a new string and copy from src using strcpy
char *copyStr(const char *src) {
    if (src == NULL) {
        return NULL;
    }
    char *dest = (char *)malloc(sizeof(char) * (strlen(src) + 1));
    if (dest == NULL) {
        perror("Error allocating string for copying");
        exit(EXIT_FAILURE);
    }

    strcpy(dest, src);
    return dest;
}

// Parses a single command from commandLine's strtok
command_t *parseCommand(char *fullCommand) {
    command_t *command = initCommand();

    command->argument[command->argc] = copyStr(strtok(fullCommand, ARGS_DELIM));
    command->commandName = command->argument[command->argc++];

    for (char *currentArg = strtok(NULL, ARGS_DELIM); currentArg != NULL && strcmp(currentArg, COMMAND_DELIM) != 0; currentArg = strtok(NULL, ARGS_DELIM)) {
        // printf("Copying str [%s]\n", currentArg);
        // Just a reminder that arguments[0] is the commandName itself, there there is actually 1 more argument;
        if (command->argc > MAX_ARGUMENTS) {
            printf("Too many arguments! Max: %d\n", MAX_ARGUMENTS);
            freeCommand(command);
            return NULL;
        }
        command->argument[command->argc++] = copyStr(currentArg);
    }

    return command;
}

// Waits and parses line from stdin
commandLine_t *parseLine(char *wholeLine, size_t lineSize) {
    // From https://stackoverflow.com/a/27491954/12921102
    do {
        printf("$vsh> ");
        if (fgets(wholeLine, lineSize, stdin) == NULL) {
            perror("Error reading from stdin");
            exit(EXIT_FAILURE);
        };
        size_t len = strlen(wholeLine);
        if (len > 0 && wholeLine[len - 1] == '\n') {
            wholeLine[--len] = '\0';
        }
    } while (wholeLine[0] == '\0');

    commandLine_t *commandLine = initCommandLine();
    // TODO build command structure while reading from stdin
    // hint: getdelim
    char *reentrantPtr;
    for (char *command = strtok_r(wholeLine, COMMAND_DELIM, &reentrantPtr); command != NULL; command = strtok_r(NULL, COMMAND_DELIM, &reentrantPtr)) {
        // printf("Parsing command [%s]\n", command);
        if (commandLine->commandc > MAX_COMMANDS) {
            printf("Too many commands! Max: %d\n", MAX_COMMANDS);
            freeCommandLine(commandLine);
            return NULL;
        }
        // If there was an error parsing arguments
        command_t *parsedCommand = parseCommand(command);
        if (parsedCommand == NULL) {
            return NULL;
        }
        commandLine->command[commandLine->commandc++] = parsedCommand;
    }

    return commandLine;
}
