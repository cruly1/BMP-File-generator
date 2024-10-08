#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>

#define VERSION "1.8"
#define DATE "2024-04-28"
#define DEV "Pávai Viktor"

#define SIZE 100
#define MAX 300

void* printVersion(){
    printf("Chart version \t%s\n", VERSION);
    return NULL;
}

void* printDate(){
    printf("Built on \t%s\n", DATE);
    return NULL;
}

void* printDeveloper(){
    printf("Developed by \t%s\n", DEV);
    return NULL;
}

void* version(){
    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, printVersion, (void*) VERSION);
    pthread_create(&thread2, NULL, printDate, (void*) DATE);
    pthread_create(&thread3, NULL, printDeveloper, (void*) DEV);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
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

int check_arguments(int argc, char *argv[], int *mode, int *connection) {
    if (argc < 2) {
        fprintf(stderr, "Error: You have to adjust switches to make your program run!\n");
        exit(2);
    }

    char *valid_args[] = {"--version", "--help", "-send", "-receive", "-file", "-socket"};
    int size = sizeof valid_args / sizeof valid_args[0];
    int ctr = 0;

    for (int i = 1; i < argc; i++) {
        for (int j = 0; j < size; j++) {
            if (strcmp(argv[i], "--version") == 0) {
                version();
                *mode = -1;
                *connection = -1;
                return EXIT_SUCCESS;
            }

            if (strcmp(argv[i], "--help") == 0) {
                help();
                *mode = -1;
                *connection = -1;
                return EXIT_SUCCESS;
            }

            if (strcmp(argv[i], valid_args[j]) == 0) {
                ctr++;
                break;
            }
        }
    }

    if (ctr != argc - 1) {
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

int Measurement(int **Values) {
    srand(time(NULL));
    time_t currentTime;
    struct tm *localTime;
    time(&currentTime);
    localTime = localtime(&currentTime);
    int min = localTime->tm_min % 15;
    int sec = localTime->tm_sec;

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
        } else {
            *(*Values + i) = *(*Values + (i - 1));
        }
    }

    return numberOfValues;
}

void BMPcreator(int *Values, int NumValues) {
    int file = open("chart.bmp", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IROTH);

    unsigned int bmpSize = 62 + ((NumValues * NumValues) / 8);

    unsigned char widthAndHeightMod = NumValues % 256;
    unsigned char widthAndHeightDiv = NumValues / 256;

    unsigned char bmpHeader[] = {
        0x42, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x3e, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, widthAndHeightMod, widthAndHeightDiv,
        0x00, 0x00, widthAndHeightMod, widthAndHeightDiv, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x0f,
        0x00, 0x00, 0x61, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xee, 0xff, 0xff, 0x00,
        0x11, 0xff};

    for (int i = 0; i < 4; ++i) {
        bmpHeader[i + 2] = (unsigned char)(bmpSize & 0xFF);
        bmpSize >>= 8;
    }

    int headerLength = sizeof(bmpHeader) / sizeof(bmpHeader[0]);
    for (int i = 0; i < headerLength; i++) {
        write(file, &bmpHeader[i], sizeof(unsigned char));
    }

    int width = NumValues, height = NumValues;

    int line = NumValues, colum = NumValues;
    if (NumValues % 32 != 0) {
        line = width + (32 - (width % 32));
        colum = height + (32 - (height % 32));
    }

    int size = (line * colum) / 8;
    unsigned char *bmpPixels = calloc(size, sizeof(unsigned char));

    int location;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            location = (i * line + j) / 8;
            if (i == Values[j] + height / 2) {
                bmpPixels[location] += (1 << (7 - (j % 8)));
            }
        }
    }

    write(file, bmpPixels, size);
    free(bmpPixels);
    close(file);
}

int FindPID() {
    int pid = -1;
    DIR *procDir;
    struct dirent *entry;
    char *proc = "/proc/";
    char currentDir[SIZE];
    char firstLine[MAX];
    char lines[MAX];
    char *token = "\t";

    procDir = opendir("/proc");
    if (procDir == NULL) {
        fprintf(stderr, "Error: Couldn't open directory.\n");
        exit(4);
    }

    while ((entry = readdir(procDir)) != NULL) {
        if (isdigit(entry->d_name[0])) {
            currentDir[0] = '\0';
            strcat(currentDir, proc);
            strcat(currentDir, entry->d_name);
            strcat(currentDir, "/status");

            FILE *fp = fopen(currentDir, "r");

            if (fp == NULL) {
                fprintf(stderr, "Error: Couldn't open file.\n");
                exit(5);
            }

            fgets(firstLine, MAX, fp);

            char* name = strtok(firstLine, token);
            name = strtok(NULL, token);

            if (strcmp(name, "chart\n") == 0) {
                while (fgets(lines, MAX, fp) != NULL) {
                    if (lines[0] == 'P' && lines[1] == 'i' && lines[2] == 'd' && lines[3] == ':' && lines[4] == '\t') {
                        char *pidString = strtok(lines, token);
                        pidString = strtok(NULL, token);
                        fclose(fp);
                        pid = atoi(pidString);

                        if (pid == getpid()) {
                            pid = -1;
                        }

                        goto end;
                    }
                }
            }

            fclose(fp);
        }
    }

end:

    closedir(procDir);

    return pid;
}

void SendViaFile(int *Values, int NumValues) {
    char* filename = "Measurements.txt";
    FILE *fp = fopen(filename, "w");

    printf("Sending %s.\n", filename);

    if (fp == NULL) {
        fprintf(stderr, "Error: Couldn't open file.\n");
        exit(5);
    }

    for (int i = 0; i < NumValues; i++) {
        fprintf(fp, "%d\n", Values[i]);
    }

    fclose(fp);

    int PID = FindPID();

    if (PID == -1) {
        fprintf(stderr, "Error: Couldn't find PID.\n");
        exit(6);
    }

    kill(PID, SIGUSR1);
    printf("Received successfully.\n");
}

void ReceiveViaFile(int sig) {
    int *Values = NULL;
    char* filename = "Measurements.txt";
    
    FILE *file = fopen(filename, "r");
    
    printf("\tReading %s.\n", filename);

    if (file == NULL) {
        fprintf(stderr, "Error: Couldn't open file\n");
        exit(5);
    }

    char line[SIZE];
    int i = 0;
    int value;
    while (fgets(line, SIZE, file) != NULL) {
        Values = realloc(Values, (i + 1) * sizeof(int));
        value = atoi(line);
        Values[i] = value;
        i++;
    }

    fclose(file);
    BMPcreator(Values, i);
    printf("Generating BMP file.\n");
    free(Values);
}

void SignalHandler(int sig) {
    if (sig == SIGINT) {
        printf("Goodbye!\n");
        exit(EXIT_SUCCESS);
    }
    if (sig == SIGUSR1) {
        fprintf(stderr, "Error: Data transfer through file is not available\n");
        exit(7);
    }
    if (sig == SIGALRM) {
        fprintf(stderr, "Error: Server currently not responding\n");
        exit(8);
    }
}

void SendViaSocket(int *Values, int NumValues) {
    int s;
    int bytes;
    int flag = 0;
    char on = 1;
    unsigned int server_size;
    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(3333);
    server_size = sizeof server;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        fprintf(stderr, "Error: Couldn't create socket.\n");
        exit(9);
    }

    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

    bytes = sendto(s, &NumValues, sizeof(NumValues), flag, (struct sockaddr *)&server, server_size);
    if (bytes <= 0) {
        fprintf(stderr, "Error: Sending error.\n");
        exit(10);
    }

    signal(SIGALRM, SignalHandler);
    sleep(1);
    signal(SIGALRM, SIG_IGN);

    int response = 0;
    bytes = recvfrom(s, &response, sizeof(int), flag, (struct sockaddr *) &server, &server_size);
    if (bytes < 0) {
        fprintf(stderr, "Error: Receiving error.\n");
        exit(11);
    }

    if (response != NumValues) {
        fprintf(stderr, "Error: Values do not match.\n");
        exit(12);
    }

    bytes = sendto(s, Values, sizeof(int) * NumValues, flag, (struct sockaddr *)&server, server_size);
    if (bytes <= 0) {
        fprintf(stderr, "Error: Sending error.\n");
        exit(10);
    }

    bytes = recvfrom(s, &response, sizeof(int), flag, (struct sockaddr *)&server, &server_size);
    if (bytes < 0) {
        fprintf(stderr, "Error: Receiving error.\n");
        exit(11);
    }

    if (response != NumValues) {
        fprintf(stderr, "Error: Values do not match.\n");
        exit(12);
    }

    close(s);
}

void ReceiveViaSocket() {
    int s;
    int err;
    int bytes;
    int flag = 0;
    char on = 1;
    int NumValues;

    unsigned int server_size;
    unsigned int client_size;

    struct sockaddr_in server;
    struct sockaddr_in client;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(3333);
    server_size = sizeof(server);
    client_size = sizeof(client);

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        fprintf(stderr, "Error: Couldn't create socket.\n");
        exit(9);
    }

    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

    err = bind(s, (struct sockaddr *) &server, server_size);
    if (err < 0) {
        fprintf(stderr, "Error: Couldn't connect to socket.\n");
        exit(13);
    }

    while (1) {
        printf("\tWaiting for data...\n");

        bytes = recvfrom(s, &NumValues, sizeof(NumValues), flag, (struct sockaddr *)&client, &client_size);
        if (bytes < 0) {
            fprintf(stderr, "Error: Receiving error.\n");
            exit(11);
        }

        bytes = sendto(s, &NumValues, sizeof(NumValues), flag, (struct sockaddr *)&client, client_size);
        if (bytes <= 0) {
            fprintf(stderr, "Error: Sending error.\n");
            exit(10);
        }
        printf("Checking array size.\n");

        int data_size = NumValues * sizeof(int);
        int *data = (int *)malloc(data_size);
        if (data == NULL) {
            fprintf(stderr, "Error: Couldn't allocate memory.");
            exit(3);
        }
        printf("Allocating array's size.\n");

        bytes = recvfrom(s, data, sizeof(int) * NumValues, flag, (struct sockaddr *)&client, &client_size);
        if (bytes < 0) {
            fprintf(stderr, "Error: Receiving error.\n");
            exit(11);
        }
        printf("Array data received.\n");

        int receivedNumValues = bytes / sizeof(int);

        bytes = sendto(s, &receivedNumValues, sizeof(int), flag, (struct sockaddr *)&client, client_size);
        if (bytes <= 0) {
            fprintf(stderr, "Error: Sending error.\n");
            exit(10);
        }
        if (receivedNumValues != NumValues) {
            fprintf(stderr, "Error: Values do not match.\n");
            exit(12);
        }
        printf("Sending received data.\n");

        BMPcreator(data, NumValues);
        free(data);
    }

    close(s);
}
