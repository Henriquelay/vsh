#include "../lib/executers.h"
#include "../lib/parser.h"
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../lib/signals.h"



int main(int argc, char *argv[]) {
    
    // numero dos sinais que tenho que bloquear 
	// 20 - SIGTSTP
	// 2 - SIGINT
	// 3 - SIGQUIT
	// Sinais que tenho que fazer com que printem na tela
	// 10 - SIGUSR1
	// 12 - SIGUSR2
	
	// criando a signal mask
	sigset_t bloqmask;

	vacinedBloq(&bloqmask);

    // struct pro SIGUSR1 e SIGUSR2
    struct sigaction sigactionStruct;
    
    vacinedAction(&sigactionStruct, &bloqmask);

    // To reuse between input reads
    char linebuffer[BUFFERSIZE];
    while (1) {
        commandLine_t *commandLine = parseLine(linebuffer, BUFFERSIZE);

        // It frees the memory after execution
        if (commandLine != NULL) {
            execCommandLine(commandLine);
        }
        commandLine = NULL;
    }
    return 0;
}
