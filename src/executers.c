#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
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

    int child_to_parent[2];
    int parent_to_child[2];

    if(pipe(child_to_parent) == -1){
        perror("\nError on pipe creation: child_to_parent");
        exit(1);
    }

    if(pipe(parent_to_child) == -1){
        perror("\nError on pipe creation: parent_to_child");
        exit(1);
    }

    pid_t *pids = malloc(sizeof(pid_t)*commands->commandc);

    for(int i=0; i<commands->commandc; i++){

        if ((i == 0) || (pids[i-1] > 0)){
            pids[i] = fork();

            if(pids[i] < 0){ // fork fail
                printf("Failed to fork. Child: %d", i);
            }else if(pids[i] == 0){ // child code => execute command

                command_t *currentCmd = commands->command[i];
                if(!commandCheck(currentCmd)){
                    printf("\nParsed command doesn't exists!");
                    return 1;
                
                }
                if(i != 0){
                    close(parent_to_child[1]);
                    dup2(parent_to_child[0], STDIN_FILENO);
                    close(parent_to_child[0]);

                }

                if ((i+1) != commands->commandc){
                    close(child_to_parent[0]);
                    dup2(child_to_parent[1], STDOUT_FILENO);
                    close(child_to_parent[1]);
                }

                if(execvp(currentCmd->commandName, currentCmd->argument) < 0){
                    printf("\nCould not execute command: %s", currentCmd->commandName);
                }

            }else{
                // parent code
            }
        }
    }

    if(pids[0] > 0){
        char buffer[BUFFERSIZE];
        int returnValue = 0;

        returnValue = read(child_to_parent[0], &buffer, BUFFERSIZE);
        if(returnValue < 0){
            printf("\nFail to read");
        }
        returnValue = write(parent_to_child[1], &buffer, strlen(buffer));
        if(returnValue < 0){
            printf("\nFail to write");
        }    
    }

    return 0;   
}
