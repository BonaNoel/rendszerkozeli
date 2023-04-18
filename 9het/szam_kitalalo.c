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
#include <string.h>

#define PORT_NO 5555

int Random_generator()
{
    return rand() % 100;
}

int Server()
{
    
    /************************ Declarations **********************/
    int s;
    int bytes;         // received/sent bytes
    int err;           // error code
    int flag;          // transmission flag
    char on;           // sockopt option
    int random_number; // datagram buffer area
    int guess;
    int result;
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
        fprintf(stderr, " Server: Socket creation error.\n");
        exit(2);
    }
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

    /************************ Binding socket ********************/
    err = bind(s, (struct sockaddr *)&server, server_size);
    if (err < 0)
    {
        fprintf(stderr, " Server: Binding error.\n");
        exit(3);
    }

    random_number = Random_generator();

    while (1)
    { // Continuous server operation
        /************************ Receive data **********************/

        bytes = recvfrom(s, &guess, sizeof(guess), flag, (struct sockaddr *)&client, &client_size);
        if (bytes < 0)
        {
            fprintf(stderr, " Server: Receiving error.\n");
            exit(4);
        }

        printf(" client (%s| |%d)\n\n ", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        printf("tipp: %d\n",guess);
        printf("Random szám: %d\n", random_number);

        if (guess == random_number)
        {
            result = 0;
            random_number = Random_generator();
        }
        else if (guess < random_number)
        {
            result = -1;
        }
        else if (guess > random_number)
        {
            result = 1;
        }

        /************************ Sending data **********************/

        bytes = sendto(s, &result, sizeof(result), flag, (struct sockaddr *)&client, client_size);
        if (bytes <= 0)
        {
            fprintf(stderr, " Server: Sending error.\n");
            exit(5);
        }
    }
}

int Client(char *ip_adress)
{
    /************************ Declarations **********************/
    int s;     // socket ID
    int bytes; // received/sent bytes
    int flag;  // transmission flag
    char on;   // sockopt option
    int n_guess;
    int n_result;              // datagram buffer area
    unsigned int server_size;  // length of the sockaddr_in server
    struct sockaddr_in server; // address of server

    /************************ Initialization ********************/
    on = 1;
    flag = 0;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip_adress);
    server.sin_port = htons(PORT_NO);
    server_size = sizeof server;

    /************************ Creating socket *******************/
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0)
    {
        fprintf(stderr, " Client: Socket creation error.\n");
        exit(2);
    }
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

    while (1)
    {

        /************************ Sending data **********************/

        fscanf(stdin, "%d", &n_guess);

        bytes = sendto(s, &n_guess, sizeof(n_guess), flag, (struct sockaddr *)&server, server_size);
        if (bytes <= 0)
        {
            fprintf(stderr, " Client: Sending error.\n");
            exit(3);
        }
        // printf("küldsé: %d", bytes);

        /************************ Receive data **********************/

        bytes = recvfrom(s, &n_result, sizeof(n_result), flag, (struct sockaddr *)&server, &server_size);
        if (bytes < 0)
        {
            fprintf(stderr, " Client: Receiving error.\n");
            exit(4);
        }

        // printf("fogadás: %d", bytes);

        if (n_result == -1)
        {
            printf("A keresett szám nagyobb mint amennyit megadtál\n");
        }
        else if (n_result == 1)
        {
            printf("A keresett szám kiseeb mint amennyit megadtál\n");
        }
        else if (n_result == 0)
        {
            printf("Eltaláltad a számot! Nyertél \n");
            break;
        }
    }
    /************************ Closing ***************************/
    close(s);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    // van parancs sori akkor kliens (ami a szerver ip cime) ha nincs akkor server

    if (argc == 1)
    {
        Server();
    }
    else
    {
        Client(argv[1]);
    }

    return 0;
}
