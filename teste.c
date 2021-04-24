#include <stdio.h>
#include "handler.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void SIGU(int sig){
if(sig == 10){
	signal(SIGUSR1,SIGU);
}

if(sig == 12){
	signal(SIGUSR2,SIGU);
}
printf("							        				_  _\n");
printf("								    _ _      (0)(0)-._  _.-'^^'^^'^^'^^'^^'--. \n");
printf("								   (.(.)----'`        ^^'                /^   ^^-._ \n");
printf("								    (    `                 \             |    _    ^^-._ \n");
printf("								    VvvvvvvVv~~`__,/.._>  /:/:/:/:/:/:/:/\  (_..,______^^-. \n");
printf("								     `^^^^^^^^`/  /   /  /`^^^^^^^^^>^^>^`>  >        _`)  ) \n");
printf("								              (((`   (((`          (((`  (((`        `'--'^ ");
printf("								I feel weird... \n");

}

int main(int argc, char* argv[]){
	while(1){
	
	// numero dos sinais que tenho que bloquear 
	// 20 - SIGTSTP
	// 2 - SIGINT
	// 3 - SIGQUIT
	// Sinais que tenho que fazer com que printem na tela
	// 10 - SIGUSR1
	// 12 - SIGUSR2
	
	// criando a signal mask
	sigset_t bloqmask;
	sigset_t prinmask;
	
	// struct pro SIGUSR1 e SIGUSR2
	struct sigaction sigactionStruct;
	sigactionStruct.sa_mask = prinmask;
	sigactionStruct.sa_handler = &SIGU;
	sigactionStruct.sa_flags = 0;
	
	// a mascara criada como vazia
	if(sigemptyset(&bloqmask) == -1){
		perror("Deu erro ao criar a mascara de bloqueio");
		return 1;
	}
	
	if(sigemptyset(&prinmask) == -1){
		perror("Deu erro ao criar a mascara de SIGUSR1 e SIGUSR2");
		return 1;
	}
	
	// adicionando os sinais que precisam ser bloqueados na mascara 1
	if(sigaddset(&bloqmask, SIGINT) == -1){
		perror("Deu erro ao adicionar o bloqueio do sinal SIGINT");
		return 2;
	}
	if(sigaddset(&bloqmask, SIGTSTP) == -1){
		perror("Deu erro ao adicionar o bloqueio do sinal SIGTSTP");
		return 2;
	}
	if(sigaddset(&bloqmask, SIGQUIT) == -1){
		perror("Deu erro ao adicionar o bloqueio do sinal SIGQUIT");
		return 2;
	}
	
	if(sigaction(SIGUSR1,&sigactionStruct, NULL) == -1){
		perror("Error na instalacao do SIGUSR1");
		return 3;
	}
	if(sigaddset(&sigactionStruct.sa_mask, SIGUSR2) == -1){
		perror("Error na instalacao do SIGUSR2");
		return 3;
	}
	signal(SIGUSR1,SIGU);
	sleep(10);
	}
	
	return 0;
}
