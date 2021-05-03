#include "../lib/executers.h"

static list_t *SIDs = NULL;

void killpgList(linked_node_t *SID) {
    // printf("Sending sig to gid %d\n", SID->value);
    if (killpg(SID->value, SIGKILL) != 0) {
        exit(EXIT_FAILURE);
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

void waitProcessOfGroups(linked_node_t *SID) {
    /*
    The waitpid() system call suspends execution of the calling thread until a child specified by pid argument has changed state.  By default, waitpid() waits only for terminated children, but this behavior is modifiable via  the options argument, as described below.

    The value of pid can be:

    < -1   meaning wait for any child process whose process group ID is equal to the absolute value of pid.
    */
    waitpid(-(SID->value), NULL, WNOHANG);
}

int execIfBultin(command_t *command) {
    int builtinValue = isBuiltin(command);
    if (builtinValue) {
        switch (command->commandName[0]) {
        case 'l': // "liberamoita"
            printf("liberamoita...\n");
            list_runOnAll(SIDs, waitProcessOfGroups);
            return 1;
            // exit(0);
        case 'a': // "armageddon"
            printf("It's the end...\n");
            // printf("List:\n");
            // printf("%p\n", (void *)SIDs->head);
            // list_print(SIDs);
            list_runOnAll(SIDs, killpgList);
            list_destroy(SIDs);
            exit(0);
        case 'e': // "exit"
            exit(0);
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
void execPiped(commandLine_t *commandLine, int fileDescriptor) {
    // Changing to new Session
    pid_t sid = setsid();
    // printf("SID from supervisor: %d\n", sid);
    if (write(fileDescriptor, &sid, sizeof(sid)) == -1) {
        perror("Erro writing on pipe");
        exit(EXIT_FAILURE);
    };
    close(fileDescriptor);
    // Initializin chidren's pipes
    pid_t pidArray[commandLine->commandc];
    int pipes[commandLine->commandc - 1][2];
    for (int i = 0; i < commandLine->commandc - 1; i++) {
        if (pipe(pipes[i]) != 0) {
            perror("Pipe error");
            exit(EXIT_FAILURE);
        }
    }
    // TODO install handler to USR1 & USR2
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
        if (waitpid(pidArray[i], &status, WIFSIGNALED(status))) { // If finished by signal
            if (WTERMSIG(status) == SIGUSR1 || WTERMSIG(status) == SIGUSR2) {
                killpg(sid, SIGKILL);
            }
        };
    }
    freeCommandLine(commandLine);
    // printf("Supervisor out! Bye!\n");
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
    int fd[2];
    if (commandLine->commandc != 1) { // Piped commands
        if (pipe(fd) != 0) {
            perror("Error on pipe to supervisor");
            exit(EXIT_FAILURE);
        }
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
            close(fd[STDIN_FILENO]);
            // Background proccess tried to take a medication, in chances of getting immunized
            takeCloroquina();
            execPiped(commandLine, fd[STDOUT_FILENO]);
        }

    } else {                              // vsh's code
        if (commandLine->commandc == 1) { // Single command
            // Wait for command to finish
            waitpid(childpid, NULL, 0);
        } else { // Piped commands
            close(fd[STDOUT_FILENO]);
            // Read from pipe
            pid_t SID;
            if (read(fd[STDIN_FILENO], &SID, sizeof(SID)) < 1) {
                perror("Error reading from pipe");
                exit(EXIT_FAILURE);
            };
            // printf("SID do supervisor sendo printado do pai: %d\n", SID);
            close(fd[STDIN_FILENO]);

            list_push(SIDs, SID);
        }
    }

    freeCommandLine(commandLine);
    return 0;
}
