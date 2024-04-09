#ifndef header
#define header
// 1. Feladat
int check_arguments(int argc, char* argv[], int* mode, int* connection);
// 2. Feladat
int Measurement(int **Values);
// 3. Feladat
void BMPcreator(int *Values, int NumValues);
// 4. Feladat
int FindPID();
// 5. Feladat
void SendViaFile(int *Values, int NumValues);
void ReceiveViaFile(int sig);
// 7. Feladat
void SignalHandler(int sig);
#endif