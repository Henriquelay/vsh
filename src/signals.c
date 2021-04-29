#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void jacarezin(int sig){
    printf("				   _  _\n");
    printf("		 _ _      (0)(0)-._  _.-'^^'^^'^^'^^'^^'--. \n");
    printf("		(.(.)----'`        ^^'       /^   ^^-._ \n");
    printf("		(    `                 \\             |    _    ^^-._ \n");
    printf("		VvvvvvvVv~~`__,/.._>  /:/:/:/:/:/:/:/\\  (_..,______^^-. \n");
    printf("		`^^^^^^^^`/  /   /  /`^^^^^^^^^>^^>^`>  >        _`)  ) \n");
    printf("				(((`   (((`          (((`  (((`        `'--'^ ");
    printf("I feel weird... \n");
}

void vacinedBloq(sigset_t *mask){
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
}

void vacinedAction(struct sigaction *sigstruct, sigset_t *mask){
    sigstruct->sa_mask = *mask;
    sigstruct->sa_handler = &jacarezin;
    sigstruct->sa_flags = 0;

    if (sigaction(SIGUSR1, sigstruct, NULL) == -1) {
        perror("Error installing sigaction\n");
        exit(EXIT_FAILURE); // Child will still be running
        // To safely end child, signal shoulg be sent, but I failed to
        // install sigaction on parent, why should I be able to do so on
        // child?
    }
    if (sigaction(SIGUSR2, sigstruct, NULL) == -1) {
        perror("Error installing sigaction\n");
        exit(EXIT_FAILURE); // Child will still be running
        // To safely end child, signal shoulg be sent, but I failed to
        // install sigaction on parent, why should I be able to do so on
        // child?
    }
}