#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#include "utils.h"

int main(int argc, char *argv[])
{
    signal(SIGINT, SignalHandler);
    signal(SIGUSR1, SignalHandler);

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
    int *Values = NULL;

    // eredmények száma
    int NumValues;

    /************ FILE  ************/
    if (is_send && is_file)
    {
        NumValues = Measurement(&Values);

        SendViaFile(Values, NumValues);

        free(Values);
    }

    if (is_receive && is_file)
    {
        while (1)
        {
            signal(SIGUSR1, ReceiveViaFile);
            pause();
        }
    }

    /************ SOCKET  ************/

    if (is_send && is_socket)
    {
        NumValues = Measurement(&Values);

        SendViaSocket(Values, NumValues);

        free(Values);
    }
    if (is_receive && is_socket)
    {
        ReceiveViaSocket();
    }

    printf("A program lefutott!\n");
    return 0;
}
