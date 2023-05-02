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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <omp.h>

#include "utils.h"

#define PORT_NO 3333

/* Megnézi hogy az állomány neve 'chart'-e */
void chart_check(char *str)
{
    // string compare
    if (strcmp(str, "./chart") != 0)
    {
        fprintf(stderr, "HIBA - nem \"chart\" az állomány neve \n");
        exit(1);
    }
}

/* Ki iratja a version információkta */
void version_argument()
{
#pragma omp parallel sections
    {
#pragma omp section
        {
            printf("Version: 1.24\n");
        }
#pragma omp section
        {
            printf("2023-04-21\n");
        }
#pragma omp section
        {
            printf("Bóna Noel\n");
        }
    }

    exit(0);
}

/* Ki iratja a help információkat */
void help_argument()
{
    printf("\nA programot a következő paraméterekkel lehet használni: \n");
    printf("\t--version: kiírja a program verzióját \n");
    printf("\t--help: kiírja a program használati utasításait\n\n");
    printf("A program üzemmódjai:\n");
    printf("\t-send: küldő üzemmód (alapértelmezett)\n");
    printf("\t-receive: fogadó üzemmód\n");
    printf("\t-file: fájlt használ a kommunikáció során (alapértelmezett)\n");
    printf("\t-socket: socketet használ a kommuniukáció során\n\n");

    exit(0);
}

/* Generál egy random double tipusu számot min és max között (zárt intervallum) */
double randfrom(double min, double max)
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

/* Feltölti az N méretű tömböt a bolyongás értékeivel */
int Measurement(int **Values)
{
    // Időkezelés
    time_t T1;
    struct tm *T2;
    int T3;

    T3 = time(&T1);
    T2 = localtime(&T1);

    // Maradékosan osztja a perceket 15-el majd megszorozza 60-al plusz hozzáadja a másodperceket
    int N = (((*T2).tm_min % 15) * 60) + (*T2).tm_sec;

    if (N < 100)
        N = 100;

    // 0.428571 x+1                 0........0.3548387096774194........0.571429..........1
    // 0.3548387096774194 x-1 --->>       x-1                   x                x + 1
    // 0.2165902903225807 x

    *Values = malloc(sizeof(int) * N);
    if (!Values)
    {
        fprintf(stderr, "HIBA - nem sikerült a memória foglalás\n");
        exit(2);
    }

    int x = 0; // Érték
    double random_number;
    int i = 0; // Index

    while (i < N)
    {
        random_number = randfrom(0, 1.0);

        // első elem 0
        if (i == 0)
            (*Values)[i] = x;
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
        i++;
    }
    return N;
}

/* Megcsinálja a .bmp fájlt */
void BMPcreator(int *Values, int NumValues)
{
    int f;
    f = open("chart.bmp", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (f < 0)
    {
        fprintf(stderr, "HIBA - Nem sikerült megnyitni / írni a chart.bmp fájlt\n");
        exit(3);
    }

    int paddingValues;

    if (NumValues % 32 == 0)
        paddingValues = NumValues;
    else
        paddingValues = NumValues + (32 - (NumValues % 32));

    int fileSize = 62 + (NumValues * paddingValues) / 8; // 62 byte + NxN(paddingel, byteba)

    unsigned char *buffer = calloc(fileSize, sizeof(char)); // calloc kinulláz mindent

    /*********** BITMAP HEADER 16 Byte *************/

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
        buffer[i] = 0x00; // 0x00 --> 00000000

    // Pixel array offset (62) 4 Bytes
    tmp = 62;
    j = 0;
    for (int i = 10; i < 14; i++)
    {
        buffer[i] = ((tmp >> (8 * j)) & 0xFF);
        j++;
    }

    /*********** DIB HEADER 40 Byte ***********/

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

    /********** Palette 8 Byte ******************/
    // color 0  All 1 Byte
    buffer[54] = 79;  // B
    buffer[55] = 69;  // G
    buffer[56] = 54;  // R
    buffer[57] = 255; // Alpha (255)

    // color 1  All 1 Byte
    buffer[58] = 0;   // B
    buffer[59] = 255; // G
    buffer[60] = 255; // R
    buffer[61] = 255; // Alpha (255)

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
        int bytebuffer = 0;
        int positon;

        // mindig 8-asával halad/néz --> byteonként pl ha i = 12 akkor az 8-és 16 között nézi
        for (int j = (i / 8) * 8; j < (i / 8) * 8 + 8; j++)
        {
            positon = 8 - (((i / 8) * 8 + 8) - j); // a 8 biten hogy melyik helyen van pl: 0 1 2 3 ... egészen 7-ig

            // az az eset amikor fölül ki menne az érték a képből
            if (Values[i] > (NumValues / 2))
            {
                if ((NumValues) % 2 == 0)
                    Values[i] = (NumValues / 2) - 1;
                else
                    Values[i] = (NumValues / 2);
            }

            // az az eset amikor alul ki menne az érték a képből (itt nem kell vizsgálni a páros páratlant)
            if (Values[i] < (NumValues / 2) * (-1))
                Values[i] = ((NumValues / 2) * (-1));

            if (Values[i] == Values[j]) // és ha van ugyanolyan értékű akkor abban a 7-ben + önmaga
            {
                bytebuffer += pow(2, 7 - positon); // beirja azt hogy 1 vagy 0 van a biten csak decimálisba (2 hatványra emel)
            }
        }
        buffer[(62 + ((paddingValues) * ((NumValues / 2) + Values[i])) / 8) + (i / 8)] = bytebuffer;
    }

    write(f, buffer, fileSize);

    close(f);
    free(buffer);
}

/* megkeresi a másik chart folyamat pid-jét (ha van) */
int FindPID()
{
    char path_buffer[300];  // elérési út megformázva
    char name_buffer[256];  // process name
    int own_pid = getpid(); // saját pid
    int pid = -1;           // keresett pid
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

            fscanf(f, "Name:\t%s\n", name_buffer); // process neve

            if (strcmp(name_buffer, "chart") == 0) // process név = a keresett névvel
            {
                fscanf(f, "%*[^\n]\n"); // elspkippel 4 sort + 1 mert az elsőt már elhagytuk
                fscanf(f, "%*[^\n]\n"); // igy a Pid-es sorra ér
                fscanf(f, "%*[^\n]\n"); // %* --> nem menti el
                fscanf(f, "%*[^\n]\n"); // [^\n]\n --> minden ami nem \n + a végén \n
                fscanf(f, "Pid:\t%d\n", &tmp);

                if (tmp != own_pid) // ha nem a saját pid akkor maradhat
                    pid = tmp;
            }
            fclose(f);
        }
    }
    closedir(d);

    return pid;
}

/* Megirja a Measurement.txt majd küld egy signált */
void SendViaFile(int *Values, int NumValues)
{
    FILE *f;
    char file_path[128];

    snprintf(file_path, sizeof(file_path), "%s/Measurement.txt", getenv("HOME"));

    // beiratás
    f = fopen(file_path, "w");
    if (f == NULL)
    {
        fprintf(stderr, "HIBA - Nem sikerült megnyitni / írni a Measurement.txt fájlt\n");
        exit(3);
    }

    for (int i = 0; i < NumValues; i++)
        fprintf(f, "%d\n", Values[i]);

    fclose((f));

    int other_pid = FindPID(); // másik process pid-ével tér vissza

    if (other_pid == -1)
    {
        fprintf(stderr, "HIBA - Nincs fájl fogadó üzenmódú folyamat\n");
        exit(4);
    }
    else
        kill(other_pid, SIGUSR1);
}

/* Beolvassa a Measurements.txt és meghívja rá a BMPcreator-t */
void ReceiveViaFile(int sig)
{
    FILE *f;
    char file_path[128];

    snprintf(file_path, sizeof(file_path), "%s/Measurement.txt", getenv("HOME"));

    // olvasás
    f = fopen(file_path, "r");
    if (f == NULL)
    {
        fprintf(stderr, "HIBA - Nem sikerült megnyitni / olvasni a Measurement.txt fájlt\n");
        exit(3);
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
    BMPcreator(Values, i);
    free(Values);
}

/* UDP-vel átküldi az értékeket (Numvalues, Values) */
void SendViaSocket(int *Values, int NumValues)
{

    /************************ Declarations **********************/
    int s;                     // socket ID
    int bytes;                 // received/sent bytes
    int flag;                  // transmission flag
    char on;                   // sockopt option
    char buffer[1024];         // datagram buffer area
    unsigned int server_size;  // length of the sockaddr_in server
    struct sockaddr_in server; // address of server

    /************************ Initialization ********************/
    on = 1;
    flag = 0;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT_NO);
    server_size = sizeof server;

    /************************ Creating socket *******************/
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0)
    {
        fprintf(stderr, "HIBA - Nem sikerült socket-et létrehozni (send)\n");
        exit(5);
    }
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

    /************************ Sending Numvalues **********************/

    // Numvalues küldése
    bytes = sendto(s, &NumValues, sizeof(NumValues), flag, (struct sockaddr *)&server, server_size);
    if (bytes <= 0)
    {
        fprintf(stderr, "HIBA - Nem sikerült az adat küldés (send)\n");
        exit(6);
    }

    // 1 másodperces időzítő hogy működik-e a komunikáció
    signal(SIGALRM, SignalHandler);
    alarm(1);

    /************************ Receive Numvalues checksum **********************/
    int checksum;
    bytes = recvfrom(s, &checksum, sizeof(checksum), flag, (struct sockaddr *)&server, &server_size);
    if (bytes < 0)
    {
        fprintf(stderr, "HIBA - Nem sikerült az adat fogadás (send)\n");
        exit(7);
    }

    alarm(0);

    if (checksum != NumValues)
    {
        fprintf(stderr, "HIBA - Cheksum érték nem megfelelő (send)\n");
        exit(8);
    }

    /************************ Sending Data Values **********************/

    int data[NumValues];

    for (int i = 0; i < NumValues; i++) // data-ba a Value ertekei hogy egybe at tudjuk küldeni
        data[i] = Values[i];

    bytes = sendto(s, data, sizeof(data), flag, (struct sockaddr *)&server, server_size);
    if (bytes <= 0)
    {
        fprintf(stderr, "HIBA - Nem sikerült az adat küldés (send)\n");
        exit(6);
    }

    checksum = bytes; // elküldött bájtok
    /************************ Recieve Data Values checksum **********************/
    int tmp;
    bytes = recvfrom(s, &tmp, sizeof(tmp), flag, (struct sockaddr *)&server, &server_size);
    if (bytes < 0)
    {
        fprintf(stderr, "HIBA - Nem sikerült az adat fogadás (send)\n");
        exit(7);
    }

    if (sizeof(data) != tmp)
    {
        fprintf(stderr, "HIBA - Cheksum érték nem megfelelő (send)\n");
        exit(8);
    }

    close(s);
}

/* UDP szerver oldal, fogadja az adotot majd meghívja rá a BMPcreator-t*/
void ReceiveViaSocket()
{
    /************************ Declarations **********************/
    int s;                     // socket ID
    int bytes;                 // received/sent bytes
    int err;                   // error code
    int flag;                  // transmission flag
    char on;                   // sockopt option
    char buffer[1024];         // datagram buffer area
    unsigned int server_size;  // length of the sockaddr_in server
    unsigned int client_size;  // length of the sockaddr_in client
    struct sockaddr_in server; // address of server
    struct sockaddr_in client; // address of client

    /************************ Initialization ********************/
    on = 1;
    flag = 0;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT_NO);
    server_size = sizeof server;
    client_size = sizeof client;

    /************************ Creating socket *******************/
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0)
    {
        fprintf(stderr, "HIBA - Nem sikerült socket-et létrehozni (recieve)\n");
        exit(5);
    }
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

    /************************ Binding socket ********************/
    err = bind(s, (struct sockaddr *)&server, server_size);
    if (err < 0)
    {
        fprintf(stderr, "HIBA - Nem sikerült kötni a socketet (receive)\n");
        exit(9);
    }

    while (1) // Continuous server operation
    {
        /************************ Receive Numvalues**********************/
        int Num_values;
        bytes = recvfrom(s, &Num_values, sizeof(Num_values), flag, (struct sockaddr *)&client, &client_size);
        if (bytes < 0)
        {
            fprintf(stderr, "HIBA - Nem sikerült az adat fogadás (receive)\n");
            exit(7);
        }

        /************************ Sending Numvalues checksum **********************/
        bytes = sendto(s, &Num_values, sizeof(Num_values), flag, (struct sockaddr *)&client, client_size);
        if (bytes <= 0)
        {
            fprintf(stderr, "HIBA - Nem sikerült az adat küldés (receive)\n");
            exit(6);
        }

        /************************ Receive data **********************/
        int *Data = malloc(sizeof(int) * Num_values); // terület lefoglalása
        if (!Data)
        {
            fprintf(stderr, "HIBA - nem sikerült a memória foglalás\n");
            exit(2);
        }

        bytes = recvfrom(s, Data, sizeof(int) * Num_values, flag, (struct sockaddr *)&client, &client_size);
        if (bytes < 0)
        {
            fprintf(stderr, "HIBA - Nem sikerült az adat fogadás (receive)\n");
            exit(7);
        }

        int checksum = bytes;

        /************************ Sending data checksum **********************/

        bytes = sendto(s, &checksum, sizeof(checksum), flag, (struct sockaddr *)&client, client_size);
        if (bytes <= 0)
        {
            fprintf(stderr, "HIBA - Nem sikerült az adat küldés (receive)\n");
            exit(6);
        }

        BMPcreator(Data, Num_values);

        free(Data);
    }
}

/* Lekezeli a signálokat */
void SignalHandler(int sig)
{
    if (sig == SIGINT)
    {
        printf("A folyamat leállítás alatt\n");
        exit(0);
    }
    else if (sig == SIGUSR1)
    {
        fprintf(stderr, " HIBA - A fájlon keresztüli küldés szolgáltatás nem elérhető\n");
        exit(10);
    }
    else if (sig == SIGALRM)
    {
        fprintf(stderr, "HIBA - A szerver nem válaszol\n");
        exit(11);
    }
}
