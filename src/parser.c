#include "../lib/parser.h"

command_t* initCommand() {
    command_t* newCommand = (command_t*)malloc(sizeof(command_t));
    if (newCommand == NULL) {
        perror("Erro allocating new command_t");
        exit(EXIT_FAILURE);
    }
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

char* copyStr(char* src) {
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
    for (int i = 0, currentArg = strtok(NULL, ARGS_DELIM); i < MAX_ARGUMENTS || currentArg == NULL; currentArg = strtok(NULL, ARGS_DELIM)) {
        command->argument[i] = copyStr(currentArg);
    }

    return command;
}

// Waits and parses line from stdin
commandLine_t* parseLine() {
    char wholeLine[BUFFERSIZE];
    scanf("%s", wholeLine);

    commandLine_t* commandLine = initCommandLine();

    char* command;
    for (int i = 0, command = strtok(wholeLine, COMMAND_DELIM); i < MAX_COMMANDS && command != NULL; i++, command = strtok(NULL, COMMAND_DELIM)) {
        commandLine->command[commandLine->commandc++] = parseCommand(command);
    }

    return commandLine;
}
