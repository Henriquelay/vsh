#include <stdio.h>
#include "../lib/parser.h"
#include "../lib/executers.h"

int main(int argc, char* argv[]) {
    while (1) {
        printf("\nvsh> ");
        commandLine_t* commandLine = parseLine();

        execCommandLine(commandLine);
        commandLine = NULL;
    }
    return 0;
}
