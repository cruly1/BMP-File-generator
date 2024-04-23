#include "header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*

send = mode 0
receive = mode 1

file = connection 0
socket = connection 1

*/

int main(int argc, char* argv[]) {

    if (strcmp(argv[0], "./chart") != 0) {
        printf("Error: Invalid filename. Filename has to be chart!\n");
        exit(1);
    }

    int mode = 0;
    int connection = 0;

    check_arguments(argc, argv, &mode, &connection);

    int *values = NULL;

    if (mode == 0 && connection) {
        int numberOfValues = Measurement(&values);
        SendViaSocket(values, numberOfValues);
        free(values);
        return EXIT_SUCCESS;
    }

    if (mode && connection) {
        ReceiveViaSocket();
        return EXIT_SUCCESS;
    }

    return 0;
}
