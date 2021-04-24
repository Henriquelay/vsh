#include <stdio.h>
#include "../lib/parser.h"
#include "../lib/executers.h"

int main(int argc, char* argv[]) {
    // To reuse between input reads
    char linebuffer[BUFFERSIZE];
    while (1) {
        commandLine_t* commandLine = parseLine(linebuffer, BUFFERSIZE);

        // It frees the memory after execution
        if(commandLine != NULL) {
            execCommandLine(commandLine);
        }
        commandLine = NULL;
    }
    return 0;
}
