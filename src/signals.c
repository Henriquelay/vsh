#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void jacarezin(int sig){
    printf("				   _  _\n");
    printf("		 _ _      (0)(0)-._  _.-'^^'^^'^^'^^'^^'--. \n");
    printf("		(.(.)----'`        ^^'       /^   ^^-._ \n");
    printf("		(    `                 \\             |    _    ^^-._ \n");
    printf("		VvvvvvvVv~~`__,/.._>  /:/:/:/:/:/:/:/\\  (_..,______^^-. \n");
    printf("		`^^^^^^^^`/  /   /  /`^^^^^^^^^>^^>^`>  >        _`)  ) \n");
    printf("				(((`   (((`          (((`  (((`        `'--'^ \n");
    printf("I feel weird... \n");
}

void coronga(int sig){
    pid_t supervisor_pid = getppid();
    kill(supervisor_pid, 34);
}


void defaultBlockMask(sigset_t *mask){
    // a mascara criada como vazia
    if (sigemptyset(mask) == -1) {
        perror("Error on block mask creation\n");
        
    }

    // adicionando os sinais que precisam ser bloqueados na mascara 1
    if (sigaddset(mask, SIGINT) == -1) {
        perror("Error on add SIGINT to mask\n");
        exit(EXIT_FAILURE);
    }
    if (sigaddset(mask, SIGTSTP) == -1) {
        perror("Error on add SIGSTP to mask\n");
        exit(EXIT_FAILURE);
    }
    if (sigaddset(mask, SIGQUIT) == -1) {
        perror("Error on add SIGQUIT to mask\n");
        exit(EXIT_FAILURE);
    }

    sigprocmask(SIG_BLOCK, mask, 0);
}

void setSigAction(struct sigaction *sigstruct, sigset_t *mask, void (*handler)(int)){
    sigstruct->sa_mask = *mask;
    sigstruct->sa_handler = handler;
    sigstruct->sa_flags = 0;

    if (sigaction(SIGUSR1, sigstruct, 0) == -1) {
        perror("Error installing sigaction\n");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGUSR2, sigstruct, 0) == -1) {
        perror("Error installing sigaction\n");
        exit(EXIT_FAILURE);
    }
}

void vaChinaAction(struct sigaction *sigstruct, sigset_t *mask){
    setSigAction(sigstruct, mask, jacarezin);
}

void cloroquinaAction(struct sigaction *sigstruct, sigset_t *mask){
    setSigAction(sigstruct, mask, coronga);
}
