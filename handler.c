#include "../lib/handler.h"

void sig_mask(int num){
	if(num == 9 || num == 19){
	printf("This signal we can't ignore!\n");	
	}
	printf("Signal -> %d ignored\n", num);

}

volatile sig_atomic_t sig1 = 0;
volatile sig_atomic_t sig2 = 0;



void handler(int signum){
  if(signum == SIGUSR1){
    sig1 = 1;	
  }else{
    sig2 = 1;
  }
}

