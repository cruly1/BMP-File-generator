#include "header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (strcmp(argv[0], "./chart") != 0) {
        printf("Error: Invalid filename. Filename has to be chart!\n");
        exit(1);
    }

    int mode = 0;
    int connection = 0;

    check_arguments(argc, argv, &mode, &connection);

    if (mode < 0 || connection < 0){
        return EXIT_SUCCESS;
    }

    signal(SIGINT,SignalHandler);
    signal(SIGUSR1,SignalHandler);

    int *Values = NULL;
    int NumValues = Measurement(&Values);

    if (mode) {
        if (connection) {
            ReceiveViaSocket();
        } else {
            while (1) {
                signal(SIGUSR1, ReceiveViaFile);
                pause();
            }
        }
    } else {
        if (connection) {
            SendViaSocket(Values, NumValues);
        } else {
            SendViaFile(Values, NumValues);
        }
    }

    free(Values);

    return 0;
}
