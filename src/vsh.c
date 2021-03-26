#include "../lib/parser.h"

int main(int argc, char* argv[]) {
    while (1) {
        printf("vsh> ");
        commandLine_t* commandLine = parseLine();



        printf("Parsed command(s):\n");
        for (int i = 0; i < commandLine->commandc; i++) {
            printf("%s:", commandLine->command[i]->commandName);
            for (int j = 0; j < commandLine->command[i]->argc; j++) {
                printf("[%s]", commandLine->command[i]->argument[j]);
            }
        }
    }
    return 0;
}
