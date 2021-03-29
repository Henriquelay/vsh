#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/parser.h"

void freeCommand(command_t *c){
    free(c->commandName);
    c->commandName = NULL;
    c->argument[0] = NULL;
    for (int i = 1; i <= c->argc; i++){
        free(c->argument[i]);
        c->argument[i] = NULL;
    }
    free(c);
}

void freeCommandLine(commandLine_t *cl){
    for (int i = 0; i < cl->commandc; i++){
        freeCommand(cl->command[i]);
        cl->command[i] = NULL;
    }
    free(cl);
}

int lineChecker(char *line, char delim, int max){
    int c = 0;
    for (int i = 0; i < BUFFERSIZE; i++){
        if(line[i] == '\0'){
            break;
        }
        if(line[i] == delim){
            if(i != 0){
                c++;
            }
        }
        if(c >= max){
            return 0;
        }
    }
    return 1;
}

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

    if(!lineChecker(fullCommand, ' ', MAX_ARGUMENTS)){
        printf("Too many arguments! Max: %d\n", MAX_ARGUMENTS);
    }else{
        command->commandName = copyStr(strtok(fullCommand, ARGS_DELIM));
        command->argument[0] = command->commandName;
        char* currentArg;
        for (currentArg = strtok(NULL, ARGS_DELIM); currentArg != NULL; currentArg = strtok(NULL, ARGS_DELIM)) {
            command->argument[(command->argc++) + 1] = copyStr(currentArg);
        }
    }

    return command;
}

// Waits and parses line from stdin
commandLine_t* parseLine() {
    // vai precisar tratar sinal pra liberar wholeLine quando o programa for morto (ctrl c, kill, etc...)
    char *wholeLine = malloc(sizeof(char) * BUFFERSIZE);
    // backup da referencia inicial pois strsep() modifica a referencia inicial
    char *wholeLineBkp = wholeLine;
    if (scanf("%[^\n]%*c", wholeLine) < 1) {
        perror("Erro scanning stdin.");
        exit(EXIT_FAILURE);
    };

    commandLine_t* commandLine = initCommandLine();

    if(!lineChecker(wholeLine, '|', MAX_COMMANDS)){
        printf("Too many commands! Max: %d\n", MAX_COMMANDS);
    }else{
        for (char* command = strsep(&wholeLine, COMMAND_DELIM); command != NULL; command = strsep(&wholeLine, COMMAND_DELIM)) {
            // printf("Tentando parsear comando %s\n", command);
            commandLine->command[commandLine->commandc++] = parseCommand(command);
        }
    }

    wholeLine = wholeLineBkp;
    free(wholeLine);
    return commandLine;
}
