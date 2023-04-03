#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#include "utils.h"

// reciev uzenmodba a signalra meghivja
void signal_handler(int sig)
{
    if (sig == SIGUSR1)
    {
        ReceiveViaFile(sig);
    }
}

int main(int argc, char *argv[])
{

    srand(time(NULL)); // 2.feladat
    // 1.feladat
    // ////////////////////////////////////////////////////////////////
    int is_send = 1; // default
    int is_receive = 0;
    int is_file = 1; // default
    int is_socket = 0;

    // ellenorzi a nevet
    chart_check(argv[0]);

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[1], "--version") == 0)
            version_argument();
        else if (strcmp(argv[1], "--help") == 0)
            help_argument();
        else if (strcmp(argv[i], "-send") == 0)
        {
            is_send = 1;
            is_receive = 0;
            continue;
        }
        else if (strcmp(argv[i], "-receive") == 0)
        {
            is_send = 0;
            is_receive = 1;
            continue;
        }
        else if (strcmp(argv[i], "-file") == 0)
        {
            is_file = 1;
            is_socket = 0;
            continue;
        }
        else if (strcmp(argv[i], "-socket") == 0)
        {
            is_file = 0;
            is_socket = 1;
            continue;
        }
        else
            help_argument();
    }

    if (is_send)
        printf("send\n");
    if (is_receive)
        printf("receive\n");
    if (is_file)
        printf("file\n");
    if (is_socket)
        printf("socket\n");

    // eredmények
    int *measurment_results = NULL;

    // eredmények száma
    int N;

    if (is_send && is_file)
    {
        N = Measurement(&measurment_results);
        printf("Ennyi szam van(mainbe(N)): %d\n",N),

        SendViaFile(measurment_results, N);

        free(measurment_results);
    }

    if (is_receive && is_file)
    {

        signal(SIGUSR1, signal_handler);
        pause();
    }

    printf("A program lefutott!\n");
    return 0;
}
