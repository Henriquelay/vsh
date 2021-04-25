#include "../lib/executers.h"

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
    const char *builtinCommands[] = {"liberamoita", "armageddon"};
    int commandNumber = 0;
    for (int i = 0; i < 2; i++) {
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
        case 'l': // "liberamoita"
            printf("liberamoita");
            exit(0);
            break;
        case 'a': // "armageddon"
            printf("armageddon");
            exit(0);
            break;
        }
    }
}

void execSingle(command_t *command) {
    execIfBultin(command);
    if (execvp(command->commandName, command->argument) < 0) {
        printf("Could not execute command: (%s)\n", command->commandName);
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
    // setsid();
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
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i != commandLine->commandc - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            execSingle(command);
        }
    }

    for (int i = 0; i < commandLine->commandc; i++) {
        waitpid(pidArray[i], NULL, 0);
    }
}

pid_t execCommandLine(commandLine_t *commandLine) {
    if (!commandLineCheck(commandLine)) {
        perror("Command is invalid\n");
        return 1;
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
            exit(EXIT_SUCCESS);
        }

    } else {                              // vsh's code
        if (commandLine->commandc == 1) { // Single command
            // Wait for command to finish
            waitpid(childpid, NULL, 0);
        } else { // Piped commands
            waitpid(childpid, NULL, WNOHANG);
        }
    }

    freeCommandLine(commandLine);
    return 0;
}
