#include "../lib/executers.h"

static list_t *SIDs = NULL;

void killpgList(linked_node_t *SID) {
    printf("Sending sig to gid %d\n", SID->value);
    if (killpg(SID->value, SIGKILL) != 0) {
        perror("Killpg failed");
        exit(EXIT_FAILURE);
    }
}

void waitSupervisors() {
    linked_node_t *actual = SIDs->head;
    while(actual != NULL) {
        linked_node_t *node = actual;
        actual = actual->next;
        if(waitpid(node->value, NULL, WNOHANG) > 0){
            killpgList(node);
            list_removeNode(SIDs, node);
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
    const char *builtinCommands[] = {"liberamoita", "armageddon", "exit", "wait"};
    int commandNumber = 0;
    for (int i = 0; i < 4; i++) {
        if (!strcmp(builtinCommands[i], command->commandName)) {
            commandNumber = i + 1;
        }
    }
    return commandNumber;
}

int execIfBultin(command_t *command) {
    int builtinValue = isBuiltin(command);
    if (builtinValue) {
        switch (command->commandName[0]) {
        // TODO
        case 'l': // "liberamoita"
            printf("liberamoita");
            exit(0);
        case 'a': // "armageddon"
        // TODO não está botando o SID na lista
        // FIXME
            printf("It's the end...\n");
            printf("List:\n");
            printf("%p\n", (void*) SIDs->head);
            list_print(SIDs);
            list_runOnAll(SIDs, killpgList);
            list_destroy(SIDs);
            exit(0);
        case 'e': // "exit"
            exit(0);
        case 'w': // "wait"
            waitSupervisors();
            return 1;
        }
    }
    return 0;
}

void execSingle(command_t *command) {
    // execIfBultin(command);
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
    // Changing to new Session
    pid_t sid = setsid();
    printf("Supervisor: My session is %d\n", sid);
    // list_push(SIDs, sid);
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
        }
    }

    closePipes(pipes, commandLine->commandc - 1, 0);
    closePipes(pipes, commandLine->commandc - 1, 1);

    // Supervisor waits for last child to finish, so it won't exit if a child is running
    int status;
    for (unsigned int i = 0; i < commandLine->commandc; i++) {
        waitpid(pidArray[i], &status, 0);
    }
    freeCommandLine(commandLine);
    // TODO: Raise SIGUSR1 or SIGUSR2 if returned on `signal`

    kill(getppid(), SIG_WAIT);
    printf("Supervisor out! Bye!\n");
    exit(EXIT_SUCCESS);
}

pid_t execCommandLine(commandLine_t *commandLine) {
    if (!commandLineCheck(commandLine)) {
        perror("Command is invalid\n");
        return 1;
    }

    if (execIfBultin(commandLine->command[0]) > 0) {
        return 0;
    };

    if (SIDs == NULL) {
        SIDs = list_init();
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

    } else {                              // vsh's code
        if (commandLine->commandc == 1) { // Single command
            // Wait for command to finish
            waitpid(childpid, NULL, 0);
        } else { // Piped commands
            while(getpgid(getpid()) ==  getpgid(childpid)){
                continue;
            }
            list_push(SIDs, getpgid(childpid));
            list_print(SIDs);
        }
    }

    freeCommandLine(commandLine);
    return 0;
}
