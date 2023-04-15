/*****************************************/
/*       - TCP server for Linux -        */
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
#define CQL     10                   // Connection Queue Length
int sc;                              // socket ID (for connects)
int s;                               // socket ID (for messages)

void stop(int sig){ // signal handler
   /************************ Closing sockets *******************/
   close(s);
   close(sc);
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
   sc = socket(AF_INET, SOCK_STREAM, 0 );
   if ( sc < 0 ) {
      fprintf(stderr, " %s: Socket creation error.\n", argv[0]);
      exit(2);
      }
   setsockopt(sc, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
   setsockopt(sc, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

   /************************ Binding socket ********************/
   err = bind(sc, (struct sockaddr *) &server, server_size);
   if ( err < 0 ) {
      fprintf(stderr," %s: Binding error.\n", argv[0]);
      exit(3);
      }

   /************************ Listening *************************/
   err = listen(sc, CQL);
   if ( err < 0 ) {
      fprintf(stderr, " %s: Listening error.\n",argv[0]);
      exit(4);
      }

   while(1){ // Continuous server operation
      /************************ Accepting connection **************/
      printf("\n Waiting for connection request...\n");
      s = accept(sc, (struct sockaddr *) &client, &client_size);
      if ( s < 0 ) {
         fprintf(stderr, " %s: Accepting error.\n",argv[0]);
         continue;
         }
      printf(" A client has connected.\n");

      /************************ Receive data **********************/
      printf(" Waiting for a message...\n");
      bytes = recv(s, buffer, BUFSIZE, flag );
      if ( bytes < 0 ) {
         fprintf(stderr, " %s: Receiving error.\n", argv[0]);
         exit(6);
         }
      printf (" %d bytes have been received from the client (%s:%d).\n Client's message:\n  %s",
               bytes-1, inet_ntoa(client.sin_addr), ntohs(client.sin_port), buffer);

      /************************ Sending data **********************/
      sprintf(buffer,"I have received a %d bytes long message.", bytes-1);
      bytes = send(s, buffer, strlen(buffer)+1, flag );
      if ( bytes < 0 ) {
         fprintf(stderr, " %s: Sending error.\n", argv[0]);
         exit(7);
         }
      printf(" Acknowledgement have been sent to client.\n");

      /************************ Closing connection ****************/
      close(s);
      }

   return EXIT_SUCCESS;
   }
