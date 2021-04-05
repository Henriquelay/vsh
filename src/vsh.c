#include <stdio.h>
#include "../lib/parser.h"
#include "../lib/executers.h"
#include "../lib/handler.h"

int main(int argc, char* argv[]) {
		struct sigaction act;
		act.sa_handler = &handler;
		act.sa_flags = 0;
		
    while (1) {
        commandLine_t* commandLine = parseLine();

        // It frees the memory after execution
        if(commandLine != NULL) {
            execCommandLine(commandLine);
        }
        commandLine = NULL;
    }
    return 0;
}
