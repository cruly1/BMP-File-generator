#include "header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    if (strcmp(argv[0], "./chart") != 0) {
        printf("Error: Invalid filename. Filename has to be chart!\n");
        exit(1);
    }

    int mode = 0;
    int connection = 0;

    int *values = NULL;

    check_arguments(argc, argv, &mode, &connection);

    if (mode == 0) {
        int numberOfValues = Measurement(&values);
        free(values);
        EXIT_SUCCESS;
    }

    return 0;
}
