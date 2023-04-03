#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>
#include <signal.h>

#include "utils.h"

void chart_check(char *str) // 1.feladat
{

    if (strstr(str, "chart") == NULL)
    {
        printf("//HIBA - nem \"chart\" az állomány neve \n");

        exit(1);
    }
}

void version_argument() // 1.feladat
{
    printf("Version: 1.0\n");
    printf("2023-03-01\n");
    printf("Pepsi Béla\n");

    exit(0);
}

void help_argument() // 1.feladat
{
    printf("Help: \n");
    printf("A programot a következő paraméterekkel lehet használni: \n");
    printf("--version: kiírja a program verzióját \n");
    printf("--help: kiírja a program használati utasításait \n");
    printf("A program üzemmódjai:\n");
    printf("-send: küldőként viselkedik (alapértelmezett)\n");
    printf("-receive: fogadóként viselkedik\n");
    printf("-file: fájlt használ a komunikáció során (alapértelmezett)\n");
    printf("-socket: socketet használ a komuniukáció során\n");

    exit(0);
}

/* generate a random floating point number from min to max */
double randfrom(double min, double max) // 2.feladat.
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

int Measurement(int **Values) // 2.feladat
{
    time_t T1;
    struct tm *T2;
    int T3;

    T3 = time(&T1);
    T2 = localtime(&T1);
    int N = (((*T2).tm_min % 15) * 60) + (*T2).tm_sec;

    if (N < 100)
    {
        N = 100;
    }

    printf("Ennyi szam van(measurement): %d\n", N);
    // 0.428571 x+1                 0........0.3548387096774194...0.571429..........1
    // 0.3548387096774194 x-1 --->>       x-1                   x           x + 1
    // 0.2165902903225807 x

    *Values = malloc(sizeof(int) * N);
    if (!Values)
    {
        write(2, "Memoria hiba!\n", 14);
        exit(1);
    }

    int x = 0;
    double random_number;
    int i = 0;

    while (i < N)
    {
        random_number = randfrom(0, 1.0);

        if (i == 0)
        {
            (*Values)[i] = x;
        }
        else
        {
            if (random_number < 0.3548387096774194)
            {
                x = x - 1;
                (*Values)[i] = x;
            }
            else if (0.3548387096774194 < random_number && random_number < 0.571429)
            {
                x = x + 0;
                (*Values)[i] = x;
            }
            else if (0.571429 < random_number)
            {
                x = x + 1;
                (*Values)[i] = x;
            }
        }

        // printf("valuse: %d \n", (*Values)[i]);
        i++;
    }

    return N;
}

void BMPcreator(int *Values, int NumValues) // 3.feladat
{
    int f;
    f = open("chart.bmp", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (f < 0)
    {
        write(2, "File error!\n", 12);
        exit(1);
    }

    int paddingValues;

    if (NumValues % 32 == 0)
    {
        paddingValues = NumValues;
    }
    else
    {
        paddingValues = NumValues + (32 - (NumValues % 32));
    }

    int fileSize = 62 + (NumValues * paddingValues) / 8; // 62 byte + NxN(paddingel, byteba)

    unsigned char *buffer = calloc(fileSize, sizeof(char)); // calloc kinullaz mindent

    // // // BITMAP HEADER 16 Byte // // //

    // Signature ("BM") 2 Byte
    buffer[0] = 'B';
    buffer[1] = 'M';

    // File size in bytes 4 Byte
    unsigned int tmp = fileSize;
    int j = 0;
    for (int i = 2; i < 6; i++)
    {
        buffer[i] = ((tmp >> (8 * j)) & 0xFF); // 0xFF --> 11111111
        j++;
    }

    //  Unused (0) 4 Byte
    for (int i = 6; i < 10; i++)
    {
        buffer[i] = 0x00; // 0x00 --> 00000000
    }

    // Pixel array offset (62) 4 Bytes
    tmp = 62;
    j = 0;
    for (int i = 10; i < 14; i++)
    {
        buffer[i] = ((tmp >> (8 * j)) & 0xFF);
        j++;
    }

    // // // DIB HEADER 40 Byte // // //

    // DIB Header size (40) 4 Byte
    tmp = 40;
    j = 0;
    for (int i = 14; i < 18; i++)
    {
        buffer[i] = ((tmp >> (8 * j)) & 0xFF);
        j++;
    }

    // Image widht (N) in pixels 4 byte
    tmp = NumValues;
    j = 0;
    for (int i = 18; i < 22; i++)
    {
        buffer[i] = ((tmp >> (8 * j)) & 0xFF);
        j++;
    }

    // Image height (N) in pixels 4 byte
    tmp = NumValues;
    j = 0;
    for (int i = 22; i < 26; i++)
    {
        buffer[i] = ((tmp >> (8 * j)) & 0xFF);
        j++;
    }

    //  Planes (1) 2 Byte
    tmp = 1;
    j = 0;
    for (int i = 26; i < 28; i++)
    {
        buffer[i] = ((tmp >> (8 * j)) & 0xFF);
        j++;
    }

    // Bits/pixel (1) 2 Byte
    tmp = 1;
    j = 0;
    for (int i = 28; i < 30; i++)
    {
        buffer[i] = ((tmp >> (8 * j)) & 0xFF);
        j++;
    }

    // Compression (0) 4 Byte
    tmp = 0;
    j = 0;
    for (int i = 30; i < 34; i++)
    {
        buffer[i] = ((tmp >> (8 * j)) & 0xFF);
        j++;
    }

    // Image size (0) 4 Byte
    tmp = 0;
    j = 0;
    for (int i = 34; i < 38; i++)
    {
        buffer[i] = ((tmp >> (8 * j)) & 0xFF);
        j++;
    }

    // Horizontal pixel/meter (3937) 4 Byte
    tmp = 3937;
    j = 0;
    for (int i = 38; i < 42; i++)
    {
        buffer[i] = ((tmp >> (8 * j)) & 0xFF);
        j++;
    }

    // Vertical pixel/meter (3937) 4 Byte
    tmp = 3937;
    j = 0;
    for (int i = 42; i < 46; i++)
    {
        buffer[i] = ((tmp >> (8 * j)) & 0xFF);
        j++;
    }

    // Colors in palette (0) 4 Byte
    tmp = 0;
    j = 0;
    for (int i = 46; i < 50; i++)
    {
        buffer[i] = ((tmp >> (8 * j)) & 0xFF);
        j++;
    }

    // Used palette colors (0) 4 Byte
    tmp = 0;
    j = 0;
    for (int i = 50; i < 54; i++)
    {
        buffer[i] = ((tmp >> (8 * j)) & 0xFF);
        j++;
    }

    // // // Palette 8 Byte // // //

    // color 0  All 1 Byte
    buffer[54] = 0;   // B
    buffer[55] = 0;   // G
    buffer[56] = 0;   // R
    buffer[57] = 255; // Alpha (255)

    // color 1  All 1 Byte
    buffer[58] = 190; // B
    buffer[59] = 100; // G
    buffer[60] = 20;  // R
    buffer[61] = 255; // Alpha (255)

    printf("Ennyi szam van(bmpcreator): %d\n", NumValues);
    printf("Ennyi szam van paddingel: %d\n", paddingValues);

    // padding values = sor   Numvalues = oszlop  kb
    // int xy = (62 + (paddingValues * (NumValues / 2) +- SOR) / 8) +- OSZLOP;
    // SOR = Values[i] oszlop = i/8
    // int zero = 62 + (paddingValues * (NumValues / 2)) / 8;
    // int one = 62 + (paddingValues * ((NumValues / 2) + 1)) / 8;
    // int two = 62 + (paddingValues * ((NumValues / 2) + 2)) / 8;
    // int three = 62 + (paddingValues * ((NumValues / 2) + 3)) / 8;
    // int four = 62 + (paddingValues * ((NumValues / 2) + 4)) / 8;
    // int five = 62 + (paddingValues * ((NumValues / 2) + 5)) / 8;
    // int six = 62 + (paddingValues * ((NumValues / 2) + 6)) / 8;
    // int seven = 62 + (paddingValues * ((NumValues / 2) + 7)) / 8;
    // int eight = (62 + ((paddingValues) * ((NumValues / 2) + 8)) / 8) + 1;

    // 0x01 <<  8 - HANYADIK BIT
    // buffer[zero] = 0X01 << 7;
    // buffer[one] = 0X01 << 6;
    // buffer[two] = 0x01 << 5;
    // buffer[three] = 0X01 << 4;
    // buffer[four] = 0X01 << 3;
    // buffer[five] = 0x01 << 2;
    // buffer[six] = 0X01 << 1;
    // buffer[seven] = 0X01 << 0;
    // buffer[eight] = 0x01 << 7;

    for (int i = 0; i < NumValues; i++)
    {
        buffer[(62 + ((paddingValues) * ((NumValues / 2) + Values[i])) / 8) + (i / 8)] = 0x01 << (8 - (i % 8));
    }

    write(f, buffer, fileSize);
    printf("\nFILESIZE: %d\n", fileSize);

    close(f);
    free(buffer);
}

int FindPID() // 4-5.feladat
{
    char looking_for[256] = "chart"; // process neve amit keresünk
    char path_buffer[300];           // elérési út megformázva
    char text_buffer[256];           // test name kiirás
    int own_pid = getpid();          // saját pid
    int pid = -1;                    // keresett pid
    int tmp = -1;
    FILE *f;
    DIR *d;
    struct dirent *entry;

    // belép a /proc mappába
    d = opendir("/proc");
    while ((entry = readdir(d)) != NULL)
    {
        // mappa neve csak szám
        if (isdigit((*entry).d_name[0]))
        {
            snprintf(path_buffer, sizeof(path_buffer), "/proc/%s/status", (*entry).d_name); // megcsinálja változóba az elérési utat

            f = fopen(path_buffer, "r");

            fscanf(f, "Name:\t%s\n", text_buffer); // process neve

            if (strcmp(text_buffer, looking_for) == 0) // process név = a keresett névvel
            {
                fscanf(f, "%*[^\n]\n"); // elspkippel 4 sort + 1 mert az elsőt már elhagytuk
                fscanf(f, "%*[^\n]\n"); // igy a Pid-es sorra ér
                fscanf(f, "%*[^\n]\n"); // %* --> nem menti el
                fscanf(f, "%*[^\n]\n"); // [^\n]\n --> minden ami nem \n + a végén \n
                fscanf(f, "Pid:\t%d\n", &tmp);

                if (tmp != own_pid) // ha nem a saját pid akkor maradhat
                {
                    pid = tmp;
                }
            }

            fclose(f);
        }
    }
    closedir(d);

    printf("%d\n", pid);
    return pid;
}

void SendViaFile(int *Values, int NumValues) // 5.feladat
{
    FILE *f;
    char file_path[128];

    snprintf(file_path, sizeof(file_path), "%s/Measurement.txt", getenv("HOME"));

    // beiratás
    f = fopen(file_path, "w");
    if (f == NULL)
    {
        fprintf(stderr, "Hiba a fájlal!\n");
        exit(1);
    }

    for (int i = 0; i < NumValues; i++)
    {
        fprintf(f, "%d\n", Values[i]);
        printf(" i: %d ",i);
    }

    fclose((f));

    int other_pid = FindPID(); // másik process pid-ével tér vissza

    if (other_pid == -1)
    {
        fprintf(stderr, "Hiba! Nincs fogadó üzenmódú folyamat!");
        exit(1);
    }
    else
    {
        kill(other_pid, SIGUSR1);
    }
}

void ReceiveViaFile(int sig) // 5.feladat
{
    FILE *f;
    char file_path[128];

    snprintf(file_path, sizeof(file_path), "%s/Measurement.txt", getenv("HOME"));

    // olvasás
    f = fopen(file_path, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Hiba a fájlal!\n");
        exit(1);
    }

    int i = 0;
    int size = 1;
    int *Values = (int *)malloc(sizeof(int));

    while (!feof(f))
    {
        if (i == size - 1)
        {
            size = size + 50;
            Values = (int *)realloc(Values, sizeof(int) * size);
        }

        fscanf(f, "%d\n", &Values[i]);
        i++;
    }

    fclose(f);

    BMPcreator(Values, i + 1);

    free(Values);
}