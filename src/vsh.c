#include <stdio.h>
#include "../lib/parser.h"
#include "../lib/executers.h"

int main(int argc, char* argv[]) {
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
