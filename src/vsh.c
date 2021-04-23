#include <stdio.h>
#include "../lib/parser.h"
#include "../lib/executers.h"
#include "../lib/handler_sig.h"

int main(int argc, char* argv[]) {
    sigset_t signalMask;
    
    struct sigaction sigStruct;
    sigStruct.sa_mask = signalMask;
    sigStruct.sa_handler = handler_signal1;
		sigStruct.sa_flags = 0;
		
		struct sigaction sigStruct;
    sigStruct.sa_mask = signalMask;
    sigStruct.sa_handler = handler_signal1;
		sigStruct.sa_flags = 0;
    
    if(sigemptyset(&signalMask) == -1){
    //&& sigaddset(&signalMask, SIGINT) == -1)
			perror("Error inic mask");
		}
    while (1) {
        commandLine_t* commandLine = parseLine();
	
	(SIGINT), Ctrl-\ (SIGQUIT), Ctrl-Z (SIGTSTP
	SIGUSR1 e SIGUSR2 

	
	
        // It frees the memory after execution
        if(commandLine != NULL) {
            execCommandLine(commandLine);
        }
        commandLine = NULL;
    }
    return 0;
}

void handler_signal1(int sig){
	signal(SIGUSR1, sigintHandler);
	vaccine_handler();
}

void handler_signal2(int sig){
	signal(SIGUSR2, sigintHandler);
	vaccine_handler();
}

void handler_signalC(int sig){
	signal(SIGINT,handler_signalC);
}

void handler_signalZ(int sig){
	signal(SIGTSTP,handler_signalZ);
}

void handler_signalY(int sig){
	signal(SIGQUIT,handler_signalY);
}
