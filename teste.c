#include <stdio.h>
#include "handler.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void SIGU(int sig){
printf("Sinal :%d recebido\n",sig);
/*
printf("							        				_  _\n");
printf("								    _ _      (0)(0)-._  _.-'^^'^^'^^'^^'^^'--. \n");
printf("								   (.(.)----'`        ^^'                /^   ^^-._ \n");
printf("								    (    `                 \             |    _    ^^-._ \n");
printf("								    VvvvvvvVv~~`__,/.._>  /:/:/:/:/:/:/:/\  (_..,______^^-. \n");
printf("								     `^^^^^^^^`/  /   /  /`^^^^^^^^^>^^>^`>  >        _`)  ) \n");
printf("								              (((`   (((`          (((`  (((`        `'--'^ ");
printf("								I feel weird... \n");
*/
sleep(5);
printf("terminei a\n");
}

int main(int argc, char* argv[]){
	
	
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
	while(1){
				printf("raise SIGUSR1\n");
        raise(SIGUSR1);
        sleep(3);
        printf("raise SIGUSR2\n");
        raise(SIGUSR2);
        sleep(3);
        printf("TESTANDO");
        printf("raise SIGINT\n");
        
    }
	
	return 0;
}
