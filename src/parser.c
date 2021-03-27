#include "../lib/parser.h"

command_t* initCommand() {
    command_t* newCommand = (command_t*)malloc(sizeof(command_t));
    if (newCommand == NULL) {
        perror("Erro allocating new command_t");
        exit(EXIT_FAILURE);
    }
    newCommand->argc = 0;
    return newCommand;
}

commandLine_t* initCommandLine() {
    commandLine_t* newCommandLine = (commandLine_t*)malloc(sizeof(commandLine_t));
    if (newCommandLine == NULL) {
        perror("Erro allocating new commandLine_t");
        exit(EXIT_FAILURE);
    }
    newCommandLine->commandc = 0;
    return newCommandLine;
}

char* copyStr(const char* src) {
    if (src == NULL) {
        return NULL;
    }
    char* dest = (char*)malloc(sizeof(char) * (strlen(src) + 1));
    if (dest == NULL) {
        perror("Erro allocating string for copying");
        exit(EXIT_FAILURE);
    }

    strcpy(dest, src);
    return dest;
}

// Parses a single command from commandLine's strtok
command_t* parseCommand(char* fullCommand) {
    command_t* command = initCommand();

    command->commandName = copyStr(strtok(fullCommand, ARGS_DELIM));
    char* currentArg;
    for (currentArg = strtok(NULL, ARGS_DELIM); currentArg != NULL; currentArg = strtok(NULL, ARGS_DELIM)) {
        command->argument[command->argc++] = copyStr(currentArg);
    }

    return command;
}

// Waits and parses line from stdin
commandLine_t* parseLine() {
    char *wholeLine = malloc(sizeof(char) * BUFFERSIZE);
    char *wholeLineBkp = wholeLine;
    if (scanf("%[^\n]%*c", wholeLine) < 1) {
        perror("Erro scanning stdin.");
        exit(EXIT_FAILURE);
    };

    commandLine_t* commandLine = initCommandLine();

    for (char* command = strsep(&wholeLine, COMMAND_DELIM); command != NULL; command = strsep(&wholeLine, COMMAND_DELIM)) {
        printf("Tentando parsear comando %s\n", command);
        commandLine->command[commandLine->commandc++] = parseCommand(command);
    }

    wholeLine = wholeLineBkp;
    free(wholeLine);
    return commandLine;
}
