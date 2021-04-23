#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h> 
#include <signal.h> // sigaction(), sigsuspend(), sig*()
#include "../lib/handler_sig.h"

void vaccine_handler() {
    printf("              _  _
    _ _      (0)(0)-._  _.-'^^'^^'^^'^^'^^'--.
   (.(.)----'`        ^^'                /^   ^^-._
    (    `                 \             |    _    ^^-._
    VvvvvvvVv~~`__,/.._>  /:/:/:/:/:/:/:/\  (_..,______^^-.
     `^^^^^^^^`/  /   /  /`^^^^^^^^^>^^>^`>  >        _`)  )
              (((`   (((`          (((`  (((`        `'--'^
I feel weird...
\n");
}

void handler_signal_block(int signal){
	return;
}




