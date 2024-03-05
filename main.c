#include "header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void kiir(int *values, int hossz) {
    for (int i = 0; i < hossz; i++) {
        if (i % 10 == 0) printf("\n");
        printf("%d\t", values[i]);
    }
}

int main(int argc, char* argv[]) {

    if (strcmp(argv[0], "./chart") != 0) {
        printf("Error: Invalid filename. Filename has to be chart!\n");
        exit(1);
    }

    int mode = 0;
    int connection = 0;

    int *values = NULL;

    // 1. Feladat
    check_arguments(argc, argv, &mode, &connection);
    // 2. Feladat
    if (mode == 0) {
        int numberOfValues = Measurement(&values);
        kiir(values, numberOfValues);
        free(values);
        EXIT_SUCCESS;
    }

    return 0;
}
