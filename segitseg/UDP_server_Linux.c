/*****************************************/
/*       - UDP server for Linux -        */
/*             by Imre Varga             */
/*****************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#define BUFSIZE 1024                 // Max length of buffer
#define PORT_NO 2222                 // The port on which the server is listening
int s;                               // socket ID

void stop(int sig){ // signal handler
   /************************ Closing ***************************/
   close(s);
   printf("\n Server is terminated.\n");
   exit(0);
   }

int main(int argc, char *argv[]){

   /************************ Declarations **********************/
   int bytes;                        // received/sent bytes
   int err;                          // error code
   int flag;                         // transmission flag
   char on;                          // sockopt option
   char buffer[BUFSIZE];             // datagram buffer area
   unsigned int server_size;         // length of the sockaddr_in server
   unsigned int client_size;         // length of the sockaddr_in client
   struct sockaddr_in server;        // address of server
   struct sockaddr_in client;        // address of client

   /************************ Initialization ********************/
   on   = 1;
   flag = 0;
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port        = htons(PORT_NO);
   server_size = sizeof server;
   client_size = sizeof client;
   signal(SIGINT, stop);
   signal(SIGTERM, stop);

   /************************ Creating socket *******************/
   s = socket(AF_INET, SOCK_DGRAM, 0 );
   if ( s < 0 ) {
      fprintf(stderr, " %s: Socket creation error.\n", argv[0]);
      exit(2);
      }
   setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
   setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

   /************************ Binding socket ********************/
   err = bind(s, (struct sockaddr *) &server, server_size);
   if ( err < 0 ) {
      fprintf(stderr," %s: Binding error.\n", argv[0]);
      exit(3);
      }

   while(1){ // Continuous server operation
      /************************ Receive data **********************/
      printf("\n Waiting for a message...\n");
      bytes = recvfrom(s, buffer, BUFSIZE, flag, (struct sockaddr *) &client, &client_size );
      if ( bytes < 0 ) {
         fprintf(stderr, " %s: Receiving error.\n", argv[0]);
         exit(4);
         }
      printf (" %d bytes have been received from the client (%s:%d).\n Client's message:\n  %s",
               bytes-1, inet_ntoa(client.sin_addr), ntohs(client.sin_port), buffer);

      /************************ Sending data **********************/
      sprintf(buffer,"I have received a %d bytes long message.", bytes-1);
      bytes = sendto(s, buffer, strlen(buffer)+1, flag, (struct sockaddr *) &client, client_size);
      if ( bytes <= 0 ) {
         fprintf(stderr, " %s: Sending error.\n", argv[0]);
         exit(5);
         }
      printf(" Acknowledgement have been sent to client.\n");
      }

   return EXIT_SUCCESS;
   }
