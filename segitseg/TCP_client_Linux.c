/*****************************************/
/*       - TCP client for Linux -        */
/*             by Imre Varga             */
/*****************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define BUFSIZE 1024                 // Max length of buffer
#define PORT_NO 2222                 // The port on which the server is listening

int main(int argc, char *argv[]){    // Server's IPv4 address is needed as command line argument

   /********************** Declarations ************************/
   int s;                            // socket ID
   int bytes;                        // received/sent bytes
   int err;                          // error code
   int flag;                         // transmission flag
   char on;                          // sockopt option
   char buffer[BUFSIZE];             // datagram buffer area
   unsigned int server_size;         // length of the sockaddr_in server
   struct sockaddr_in server;        // address of server

   /********************** Initialization **********************/
   on   = 1;
   flag = 0;
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = inet_addr(argc==1 ? "127.0.0.1" : argv[1]);
   server.sin_port        = htons(PORT_NO);
   server_size = sizeof server;

   /********************** Creating socket *********************/
   s = socket(AF_INET, SOCK_STREAM, 0 );
   if ( s < 0 ) {
      fprintf(stderr, " %s: Socket creation error.\n", argv[0]);
      exit(2);
      }
   setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
   setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

   /********************** Connecting **************************/
   err = connect(s, (struct sockaddr *) &server, server_size);
   if ( err < 0 ) {
      fprintf(stderr, " %s: Connecting error.\n", argv[0]);
      exit(3);
      }
   printf(" Connection is OK.\n");

   /********************** Sending data ************************/
   printf(" Message to send: ");
   fgets(buffer,BUFSIZE,stdin);
   bytes = send(s, buffer, strlen(buffer)+1, flag);
   if ( bytes <= 0 ) {
      fprintf(stderr, " %s: Sending error.\n", argv[0]);
      exit(4);
      }
   printf (" %i bytes have been sent to server.\n", bytes-1);

   /********************** Receive data ************************/
   bytes = recv(s, buffer, BUFSIZE, flag);
   if ( bytes < 0 ) {
      fprintf(stderr, " %s: Receiving error.\n", argv[0]);
      exit(5);
      }
   printf(" Server's (%s:%d) acknowledgement:\n  %s\n",
           inet_ntoa(server.sin_addr), ntohs(server.sin_port), buffer);

   /********************** Closing *****************************/
   close(s);

   return EXIT_SUCCESS;
   }
