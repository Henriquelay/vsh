/**
 * This library handles with signal masks and signal handlers
 **/
#ifndef _VSH_SIGNALS_H_
#define _VSH_SIGNALS_H_

#include <signal.h>

void vacinedBloq(sigset_t *blockmask);

void vacinedAction(struct sigaction *sigstruct, sigset_t *mask);

#endif
