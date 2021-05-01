/**
 * This library handles with signal masks and signal handlers
 **/
#ifndef _VSH_SIGNALS_H_
#define _VSH_SIGNALS_H_

#define SIG_WAIT (SIGRTMIN+2)

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "./executers.h"

// void defaultBlockMask(sigset_t *blockmask);

// void setSigAction(struct sigaction *sigstruct, sigset_t *mask, void (*handler)(int));

void takeVaChina();

void installWaiter();

void cloroquinaAction(struct sigaction *sigstruct, sigset_t *mask);

#endif
