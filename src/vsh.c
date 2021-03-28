#include <stdio.h>
#include "../lib/parser.h"
#include "../lib/executers.h"
#include <signal.h>
#include "../lib/handlerSignal.h"


int main(int argc, char* argv[]) {
    while (1) {
        printf("\nvsh> ");
        commandLine_t* commandLine = parseLine();

        execCommandLine(commandLine);
    }
    return 0;
}
