#include "../lib/executers.h"
#include "../lib/parser.h"
#include "../lib/signals.h"

int main(int argc, char *argv[]) {
    // Vsh has been imunized by a weird immunoagent from the east
    takeVaChina();
    

    // Waiter on signals to prevent zombies
    // installWaiter();

    // To reuse between input reads
    char linebuffer[BUFFERSIZE];
    while (1) {
        commandLine_t *commandLine = parseLine(linebuffer, BUFFERSIZE);

        // It frees the memory after execution
        if (commandLine != NULL) {
            execCommandLine(commandLine);
        }
        commandLine = NULL;
    }
    return 0;
}
