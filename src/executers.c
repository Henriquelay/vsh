#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include "../lib/executers.h"

int commandCheck(command_t *command){
    if(command == NULL){
        return 0;
    }else{
        if(command->commandName == NULL){
            return 0;
        }else{
            if(!strcmp(command->commandName, "")){
                return 0;
            }
        }
    }
    return 1;
}

int execCommandLine(commandLine_t *commands){
    int returnValue = 0;
    if(commands->commandc == 1){        
        returnValue = execSingle(commands->command[0]);
    }else if(commands->commandc > 1){
        returnValue = execPiped(commands);
    }

    freeCommandLine(commands);
    return returnValue;
}

int execBultin(command_t *command){
    const char *builtinCommands[] = {"liberamoita", "armageddon"};
    int commandNumber = -1;
    for(int i=0; i<2; i++){
        if(!strcmp(builtinCommands[i], command->commandName)){
            commandNumber = i;
        }
    }

    switch (commandNumber)
    {
    case 0:
        printf("liberamoita");
        return 0;
        break;
    case 1:
        printf("armageddon");
        return 0;
        break;
    
    default:
        return -1;
        break;
    }
}

int execSingle(command_t *command){

    if(!commandCheck(command)){
        printf("\nParsed command doesn't exists!");
        return 1;
    }

    int returnValue = execBultin(command);
    if(returnValue != -1){
        return returnValue;
    }

    pid_t pid = fork();

    if(pid == -1){
        printf("Failed to fork. Command: %s", command->commandName);
        return 1;
    }else if (pid == 0){
        if(execvp(command->commandName, command->argument) < 0){
            printf("\nCould not execute command: %s", command->commandName);
        }
        exit(0);
    }else{
        wait(NULL);
        return 0;
    }    
}

int execPiped(commandLine_t *commands){
    return 0;   
}