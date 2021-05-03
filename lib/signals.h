/**
 * This library handles with signal masks and signal handlers
 **/
#ifndef _VSH_SIGNALS_H_
#define _VSH_SIGNALS_H_

#define SIG_WAIT (SIGRTMIN + 2)

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "./executers.h"

void takeVaChina();

void takeCloroquina();

#endif
