#include <stdio.h>
#include "../lib/parser.h"
#include "../lib/executers.h"
#include <signal.h>
#include <wait.h>

pid_t pid;
int counter= 0;

void sig_handler(int signum){
	counter++;
	printf("Estou dentro do handler signal.\n");
	printf("contador = %d  ", counter);
	printf("Foi enviado o signal %d\n", signum);
}

