/**
 * This library handles with signal masks and signal handlers
 **/
#ifndef _VSH_SIGNALS_H_
#define _VSH_SIGNALS_H_

#include <signal.h>

void defaultBlockMask(sigset_t *blockmask);

void setSigAction(struct sigaction *sigstruct, sigset_t *mask, void (*handler)(int));

void vaChinaAction(struct sigaction *sigstruct, sigset_t *mask);

void cloroquinaAction(struct sigaction *sigstruct, sigset_t *mask);

#endif
