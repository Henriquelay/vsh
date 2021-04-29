#include "../lib/executers.h"
#include "../lib/parser.h"
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void SIGU(int sig){
printf("				   _  _\n");
printf("		 _ _      (0)(0)-._  _.-'^^'^^'^^'^^'^^'--. \n");
printf("		(.(.)----'`        ^^'       /^   ^^-._ \n");
printf("		(    `                 \             |    _    ^^-._ \n");
printf("		VvvvvvvVv~~`__,/.._>  /:/:/:/:/:/:/:/\  (_..,______^^-. \n");
printf("		`^^^^^^^^`/  /   /  /`^^^^^^^^^>^^>^`>  >        _`)  ) \n");
printf("				(((`   (((`          (((`  (((`        `'--'^ ");
printf("I feel weird... \n");
}

int main(int argc, char *argv[]) {
    // To reuse between input reads
    
    sigset_t bloqmask;
    // numero dos sinais que tenho que bloquear 
	// 20 - SIGTSTP
	// 2 - SIGINT
	// 3 - SIGQUIT
	// Sinais que tenho que fazer com que printem na tela
	// 10 - SIGUSR1
	// 12 - SIGUSR2
	
	// criando a signal mask
	sigset_t bloqmask;

	

	// a mascara criada como vazia
        if (sigemptyset(&bloqmask) == -1) {
            perror("Deu erro ao criar a mascara de bloqueio");
            return 1;
        }

        ///////
        // adicionando os sinais que precisam ser bloqueados na mascara 1
        if (sigaddset(&bloqmask, SIGINT) == -1) {
            perror("Deu erro ao adicionar o bloqueio do sinal SIGINT");
            return 2;
        }
        if (sigaddset(&bloqmask, SIGTSTP) == -1) {
            perror("Deu erro ao adicionar o bloqueio do sinal SIGTSTP");
            return 2;
        }
        if (sigaddset(&bloqmask, SIGQUIT) == -1) {
            perror("Deu erro ao adicionar o bloqueio do sinal SIGQUIT");
            return 2;
        }
        // struct pro SIGUSR1 e SIGUSR2
        struct sigaction sigactionStruct;
        sigactionStruct.sa_mask = bloqmask;
        sigactionStruct.sa_handler = &SIGU;
        sigactionStruct.sa_flags = 0;

        if (sigaction(SIGUSR1, &sigactionStruct, NULL) == -1) {
            perror("Error installing sigaction");
            exit(EXIT_FAILURE); // Child will still be running
            // To safely end child, signal shoulg be sent, but I failed to
            // install sigaction on parent, why should I be able to do so on
            // child?
        }
        if (sigaction(SIGUSR2, &sigactionStruct, NULL) == -1) {
            perror("Error installing sigaction");
            exit(EXIT_FAILURE); // Child will still be running
            // To safely end child, signal shoulg be sent, but I failed to
            // install sigaction on parent, why should I be able to do so on
            // child?
        }
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
