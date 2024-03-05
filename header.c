#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VERSION 1.2
#define DEV "Pávai Viktor"

void version() {
    printf("Version: %.1f\n", VERSION);
    printf("Developed by: %s\n", DEV);
}

void help() {
    printf("Options:\n");
    printf("    --help        Lists the arguments that can be used\n");
    printf("    --version     It lists the version of the program, the name of the creator and the last day of modification\n");
    printf("    -send         Sets the program to send mode (default)\n");
    printf("    -receive      Sets the program to receive mode\n");
    printf("    -file         Set the sending method to file-based (default)\n");
    printf("    -socket       Set the sending method to socket-based\n");
}

// 1. Feladat
int check_arguments(int argc, char* argv[], int* mode, int* connection) {
    if (argc < 2) {
        printf("Error: You have to adjust switches to make your program run!\n");
        exit(2);
    }

    char* valid_args[] = {"--version", "--help", "-send", "-receive", "-file", "-socket"};
    int size = sizeof valid_args / sizeof valid_args[0];
    int ctr = 0;

    for (int i = 1; i < argc; i++) {
        for (int j = 0; j < size; j++) {
            if (strcmp(argv[i], "--version") == 0) {
                version();
                return EXIT_SUCCESS;
            }

            if (strcmp(argv[i], "--help") == 0) {
                help();
                return EXIT_SUCCESS;
            }

            if (strcmp(argv[i], valid_args[j]) == 0) {
                ctr++;
                break;
            }
        }
    }

    if (ctr != argc-1) {
        help();
        return EXIT_SUCCESS;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-send") == 0) {
            *mode = 0;
        }
        if (strcmp(argv[i], "-receive") == 0) {
            *mode = 1;
        }
        if (strcmp(argv[i], "-file") == 0) {
            *connection = 0;
        }
        if (strcmp(argv[i], "-socket") == 0) {
            *connection = 1;
        }
    }

    return EXIT_SUCCESS;
}

// 2. Feladat
int Measurement(int **Values) {
    srand(time(NULL));
    time_t currentTime;
    struct tm *localTime;
    time(&currentTime);
    localTime = localtime(&currentTime);
    int min = localTime->tm_min;
    int sec = localTime->tm_sec;

    while (min >= 15) {
        min -= 15;
    }

    int numberOfValues = (min * 60 + sec) > 100 ? min * 60 + sec : 100;
    *Values = (int *)malloc(numberOfValues * sizeof(int));

    if (Values == NULL) {
        fprintf(stderr, "Error: Couldn't allocate memory.\n");
        exit(3);
    }

    *(*Values + 0) = 0;
    double random;

    for (int i = 1; i < numberOfValues; i++) {
        random = ((double)rand() / RAND_MAX) * 100.0;

        if (random <= 42.8571) {
            *(*Values + i) = *(*Values + (i - 1)) + 1;
        } else if (random <= 78.341) {
            *(*Values + i) = *(*Values + (i - 1)) - 1;
        } else  {
            *(*Values + i) = *(*Values + (i - 1));
        }
    }

    return numberOfValues;
}
