#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

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
        perror("malloc-hiba");
        exit(1);
    }

    int x = 0;
    double random_number;
    int i = 0;

    while (i < N)
    {
        random_number = randfrom(0, 1.0);
        // printf("random: %f\n", random_number);

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
        write(2, "error", 5);
        exit(1);
    }

    // BITMAP HEADER 16B
    write(f, "BM", 2); // BM 2B
    unsigned int file_size = 62 + NumValues * NumValues;
    write(f, file_size, 4);                          // file size in bytes 4B (62B+pixeltömb mérete)
    write(f, 0b00000000000000000000000000000000, 4); // unused(0) 4B
    unsigned int pixel_array_offset = 62;
    write(f, pixel_array_offset, 4); // pixel array offset 4B

    // DIB HEADER 40B
    write(f, 40u, 4); // DIB header size 4B
    unsigned int row, collumn = NumValues;
    write(f, row, 4); // image widht in pixels 4B
    write(f, row, 4); // image height in pixels 4B // NxN-es == NxM
    unsigned int egy = 1;
    write(f, egy, 2);                                // planes(1) 2B
    write(f, egy, 2);                                // Bbts/pixel(1) 2B
    write(f, 0b00000000000000000000000000000000, 4); // compression(0) 4B
    write(f, 0b00000000000000000000000000000000, 4); // image size(0) 4B
    unsigned int pixel_meter = 3937;
    write(f, pixel_meter, 4);                        // horizontal pixel/meter(3937) 4B
    write(f, pixel_meter, 4);                        // vertical pixel/meter(3937) 4B
    write(f, 0b00000000000000000000000000000000, 4); // colors in palette(0) 4B
    write(f, 0b00000000000000000000000000000000, 4); // used palette colors(0) 4B

    // PALETTE 8B
    write(f, 0, 1);   // color 0 B 1B FEKETE
    write(f, 0, 1);   // color 0 G 1B
    write(f, 0, 1);   // color 0 R 1B
    write(f, 255, 1); // color 0 Alpha(255) 1B

    write(f, 255, 1); // color 1 B 1B FEHÉR
    write(f, 255, 1); // color 1 G 1B
    write(f, 255, 1); // color 1 R 1B
    write(f, 255, 1); // color 1 Alpha(255) 1B

    // PIXEL ARRAY ?B NumValuse=row=collumn

    unsigned int pixel_aray[row][collumn];
    unsigned int zero = collumn / 2; // referencia 0 sor

    unsigned int padding;
    if (NumValues % 32 == 0)
        padding = 0;
    else
        padding = 32 - (NumValues % 32);

    close(f);
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

        for (int i = 0; i < N; i++)
        {
            printf("%d ", results[i]);
        }
        printf("\n");

        /////////////////////////////////////////////////////////////////

        // 3.feladat
        /////////////////////////////////////////////////////////////////

        BMPcreator(results, N);
    }
    printf("A program lefutott!\n");
    return 0;
}
