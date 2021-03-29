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
        waitpid(pid, NULL, 0);
        return 0;
    }    
}


// write do child vai no read do parent (usando o pipe child_to_parent)
// write do parent vai no read do child (usando o pipe parent_to_child)
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

    pid_t pids[commands->commandc];

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

                close(parent_to_child[1]);
                if(i != 0){
                    dup2(parent_to_child[0], STDIN_FILENO);
                }
                close(parent_to_child[0]);


                close(child_to_parent[0]);
                if ((i+1) != commands->commandc){
                    dup2(child_to_parent[1], STDOUT_FILENO);
                }
                close(child_to_parent[1]);

                if(execvp(currentCmd->commandName, currentCmd->argument) < 0){
                    printf("\nCould not execute command: %s", currentCmd->commandName);
                }

            }else{
                char buffer[BUFFERSIZE];
                int returnValue = 0;

                if((i+1) != commands->commandc){
                    returnValue = read(child_to_parent[0], &buffer, BUFFERSIZE);
                    if(returnValue < 0){
                        printf("\nFail to read");
                    }
                    returnValue = write(parent_to_child[1], &buffer, strlen(buffer));
                    if(returnValue < 0){
                        printf("\nFail to write");
                    }
                }
                /* 
                # TODO
                Testando com o comando "ls -la | grep vsh": 
                - O "ls -la" executa
                - o pai recebe a saída de "ls -la"
                - repassa a saída pro "grep vsh"
                - o "grep vsh" recebe a entrada repassada e printa o resultado no STDOUT do programa
                - porém "grep vsh" fica como stopped e não morre, travando o programa
                - waitpid(pids[i], NULL, WNOHANG); "resolve" o problema de travamento, porém, ocorre o seguinte:
                    \_ ./vsh
                        \_ [ls] <defunct>
                        \_ grep vsh
                */
                waitpid(pids[i], NULL, WNOHANG); 
            }
        }
    }



    return 0;   
}