/**
* This library is treating signals
**/

#ifndef _VSH_HANDLER_SIGNAL_H_
#define _VSH_HANDLER_SIGNAL_H_

// Declared in assignment's specification
#define SIGHUP 1 	/* Hangup the process */
#define SIGINT 2 	/* Interrupt the process */
#define SIGQUIT 3 	/* Quit the process */
#define SIGILL 4 	/* Illegal instruction */
#define SIGTRAP 5 	/* Trace Trap */
#define SIGABRT 6 	/* Abort */

#define BUFFERSIZE (1000)


void sig_handler(int signum);


#endif
