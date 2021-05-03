#include "../lib/signals.h"

void jacarezin(int sig) {
    printf("	   _  _\n _ _      (0)(0)-._  _.-'^^'^^'^^'^^'^^'--. \n(.(.)----'`        ^^'                /^   ^^-._ \n(    `                 \\             |    _    ^^-._ \nVvvvvvvVv~~`__,/.._>  /:/:/:/:/:/:/:/\\  (_..,______^^-. \n`^^^^^^^^`/  /   /  /`^^^^^^^^^>^^>^`>  >        _`)  ) \n         (((`   (((`          (((`  (((`        `'--'^ \nI feel weird... \n");
}

void corongaHandler(int sig) {
    // printf("Corongando caralho fodase\n");
    killpg(getsid(0), SIGKILL);
}

void defaultBlockMask(sigset_t *mask) {
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

void takeVaChina() {
    sigset_t bloqmask;

    defaultBlockMask(&bloqmask);

    struct sigaction sigstruct;
    sigstruct.sa_mask = bloqmask;
    sigstruct.sa_handler = jacarezin;
    sigstruct.sa_flags = 0;

    if (sigaction(SIGUSR1, &sigstruct, 0) == -1) {
        perror("Error installing sigaction USR1\n");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGUSR2, &sigstruct, 0) == -1) {
        perror("Error installing sigaction USR2\n");
        exit(EXIT_FAILURE);
    }
}

void takeCloroquina() {
    sigset_t bloqmask;

    sigemptyset(&bloqmask);

    struct sigaction sigstruct;
    sigstruct.sa_mask = bloqmask;
    sigstruct.sa_handler = corongaHandler;
    sigstruct.sa_flags = 0;

    if (sigaction(SIGUSR1, &sigstruct, 0) == -1) {
        perror("Error installing sigaction USR1\n");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGUSR2, &sigstruct, 0) == -1) {
        perror("Error installing sigaction USR2\n");
        exit(EXIT_FAILURE);
    }
}
