#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../lib/executers.h"
#include "../lib/list.h"

list_t *Supervisors = NULL;
list_t *Pids = NULL;

void wait_pids(int sig){
    linked_node_t *current = Supervisors->head;

    // clean supervisors
    while(current != NULL){
        if(waitpid(current->value, NULL, WNOHANG) > 0){
            list_remove(Supervisors, current->value);
            break;
        }
        current = current->next;
    }

    current = Pids->head;

    // clean childs
    while(current != NULL){
        pid_t pid = current->value;
        current = current->next;
        if(waitpid(pid, NULL, WNOHANG) > 0){
            list_remove(Pids, pid);
        }
    }
}

int commandLineCheck(commandLine_t *commandLine) {
    if (commandLine != NULL) {
        for (int i = 0; i < commandLine->commandc; i++) {
            command_t *command = commandLine->command[i];
            if (command != NULL && command->commandName != NULL &&
                strcmp(command->commandName, "") != 0) {
                continue;
            } else {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

int isBuiltin(command_t *command) {
    const char *builtinCommands[] = {"liberamoita", "armageddon", "exit"};
    int commandNumber = 0;
    for (int i = 0; i < 3; i++) {
        if (!strcmp(builtinCommands[i], command->commandName)) {
            commandNumber = i + 1;
        }
    }
    return commandNumber;
}

void execIfBultin(command_t *command) {
    int builtinValue = isBuiltin(command);
    if (builtinValue) {
        switch (command->commandName[0]) {
        // TODO
        case 'l': // "liberamoita"
            printf("liberamoita");
            exit(0);
            break;
        case 'a': // "armageddon"
            // TODO: não esquecer de destruir a lista de pids e supervisors nesse comando
            printf("armageddon"); 
            exit(0);
            break;
        case 'e': // "exit"
            kill(getppid(), SIGKILL);
            exit(0);
            break;
        }
    }
}

void execSingle(command_t *command) {
    execIfBultin(command);
    printf("passou daqui");
    if (execvp(command->commandName, command->argument) == -1) {
        perror("Error executing command");
        printf("Could not execute command: (%s)\n", command->commandName);
        exit(EXIT_FAILURE);
    }
}

void closePipes(int pipes[][2], unsigned int pipesCount, unsigned int desc) {
    for (unsigned int i = 0; i < pipesCount; i++) {
        if (close(pipes[i][desc]) == -1) {
            perror("Error closing pipe");
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * Will create a tree of process, one piped directly to another:
 *            SV
 *           /|\
 *          A B C
 * Supervisor is kept to receive signals and treat the group os process
 * */
void execPiped(commandLine_t *commandLine) {
    // Seting SV to another session
    setsid();
    // Initializin chidren's pipes
    pid_t pidArray[commandLine->commandc];
    int pipes[commandLine->commandc - 1][2];
    for (int i = 0; i < commandLine->commandc - 1; i++) {
        if (pipe(pipes[i]) != 0) {
            perror("Pipe error");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < commandLine->commandc; i++) {
        command_t *command = commandLine->command[i];
        // Pipe is accesible on parent and child
        pidArray[i] = fork();
        if (pidArray[i] == -1) {
            perror("Fork error");
            exit(EXIT_FAILURE);
        }

        if (pidArray[i] == 0) { // Child's code
            // Sets input to pipe's output and goes to next for iteration
            if (i != 0) { // Not first
                if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1) {
                    perror("Dup2 error");
                    exit(EXIT_FAILURE);
                }
            }
            if (i != commandLine->commandc - 1) { // Not last
                if (dup2(pipes[i][1], STDOUT_FILENO) == -1) {
                    perror("Dup2 error");
                    exit(EXIT_FAILURE);
                }
            }

            closePipes(pipes, commandLine->commandc - 1, 0);
            closePipes(pipes, commandLine->commandc - 1, 1);


            execSingle(command);
        }else{
            list_push(Pids, pidArray[i]); // esse inferno n ta aparecendo na lista mesmo depois do push
        }
    }

    freeCommandLine(commandLine);
    closePipes(pipes, commandLine->commandc - 1, 0);
    closePipes(pipes, commandLine->commandc - 1, 1);
    // Supervisor waits for last child to finish, so it won't exit if a child is running
    int status;
    for (unsigned int i = 0; i < commandLine->commandc; i++) {
        waitpid(pidArray[i], &status, 0);
    }
    // TODO: Raise SIGUSR1 or SIGUSR2 if returned on `signal`
    // TODO: Raise "finished" signal to main before exiting, so it can run waiting routine
    exit(EXIT_SUCCESS);
}

pid_t execCommandLine(commandLine_t *commandLine) {
    if (!commandLineCheck(commandLine)) {
        perror("Command is invalid\n");
        return 1;
    }

    if(Pids == NULL) {
        Pids = list_init();
    }

    if(Supervisors == NULL) {
        Supervisors = list_init();
    }

    pid_t childpid = fork();
    if (childpid == -1) {
        perror("Failed to fork. Exiting\n");
        exit(EXIT_FAILURE);
    }
    if (childpid == 0) {                  // Supervisor's code
        if (commandLine->commandc == 1) { // Single command
            // "Supervisor" becomes the process who runs the command itself
            execSingle(commandLine->command[0]);
        } else { // Piped commands
            execPiped(commandLine);
        }

    } else { // vsh's code
        if (commandLine->commandc == 1) { // Single command
            list_push(Pids, childpid);
            // Wait for command to finish
            waitpid(childpid, NULL, 0);
            list_remove(Pids, childpid);
        } else { // Piped commands
            if(waitpid(childpid, NULL, WNOHANG) <= 0){
                // list_push(Supervisors, childpid);
            }
        }
        printf("childs: ");
        list_print(Pids);
        printf("supervisors: ");
        list_print(Supervisors);
    }

    freeCommandLine(commandLine);
    return 0;
}
