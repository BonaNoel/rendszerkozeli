#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>

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

    int paddingValues; // NumValues + padding

    if (NumValues % 32 == 0)
    {
        paddingValues = NumValues;
    }
    else
    {
        paddingValues = NumValues + (32 - (NumValues % 32));
    }

    int fileSize = 62 + (NumValues * paddingValues) / 8; // 62 byte + NxN(paddingel, byteba)

    unsigned char *buffer = calloc(fileSize, sizeof(char));

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
    buffer[54] = 50;  // B
    buffer[55] = 150; // G
    buffer[56] = 40;  // R
    buffer[57] = 255; // Alpha (255)

    // color 1  All 1 Byte
    buffer[58] = 155; // B
    buffer[59] = 100; // G
    buffer[60] = 20;  // R
    buffer[61] = 255; // Alpha (255)

    printf("Ennyi szam van: %d\n", NumValues);

    int zero = NumValues / 2;
    printf("Nulla sor: %d\n", zero);

    for (int i = 62; i < fileSize; i++)
    {
    }

    write(f, buffer, fileSize);
    printf("\n%d\n", fileSize);

    close(f);
    free(buffer);
}

int FindPID() // 4-5.feladat
{
    char looking_for[256] = "fish\0";
    char path_buffer[300]; // elérési út megformázva
    char text_buffer[256]; // test name kiirás
    int p = -1;            // pid
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
                fscanf(f, "Pid:\t%d\n",&p);
            }

            fclose(f);
        }
    }
    closedir(d);

    printf("%d\n", p);
    return p;
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
    /////////////////////////////////////////////////////////////////

    // 2.feladat
    /////////////////////////////////////////////////////////////////
    int *results = NULL;

    int N;

    if (is_send)
    {
        N = Measurement(&results);

        // for (int i = 0; i < N; i++)
        // {
        //     printf("%d ", results[i]);
        // }
        // printf("\n");

        /////////////////////////////////////////////////////////////////

        // 3.feladat
        /////////////////////////////////////////////////////////////////

        BMPcreator(results, N);
    }

    /////////////////////////////////////////////////////////////////

    // 4-5.feladat
    /////////////////////////////////////////////////////////////////

    FindPID();

    printf("A program lefutott!\n");
    return 0;
}
