#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#define SA struct sockaddr
/* You will to add includes here */

// Enable if you want debugging to be printed, see examble below.
// Alternative, pass CFLAGS=-DDEBUG to make, make CFLAGS=-DDEBUG
#define DEBUG


// Included to get the support library
#include <calcLib.h>

int main(int argc, char *argv[]){

  char delim[]=":";
  char *Desthost=strtok(argv[1],delim);
  char *Destport=strtok(NULL,delim);
  struct sockaddr_in servaddr;
  char server_message[2000], client_message[2000];
  // *Desthost now points to a sting holding whatever came before the delimiter, ':'.
  // *Dstport points to whatever string came after the delimiter. 

  /* Do magic */
  int port=atoi(Destport);
  
  //Create TCP socket
  int socket_desc;
  struct sockaddr_in server_addr;
  
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  
  if(socket_desc < 0){
  	#ifdef DEBUG
  	printf("Unable to create socket\n");
  	#endif
  	return -1;
  }
  #ifdef DEBUG
  else printf("Socket Created\n");
  #endif
  
  
#ifdef DEBUG 
  printf("Host %s, and Port %d.\n",Desthost,port);
#endif


  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(Desthost);
  
  int error;
  error = connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(error < 0){
  	#ifdef DEBUG
  	printf("Unable to connect\n");
  	printf("Error: %d \n", errno);
  	#endif
  	return -1;
  }
  #ifdef DEBUG
  else printf("Connected\n");
  #endif
  
  if(recv(socket_desc, server_message, sizeof(server_message), 0) < 0){
  	#ifdef DEBUG
  	printf("Error receiving message\n");
  	#endif
  	return -1;
  }
  else printf(server_message);
  
  if(strcmp(server_message,"TEXT TCP 1.0\n\n") == 0){
  	#ifdef DEBUG
  	printf("Same\n");
  	#endif
  }
  //else return -1;
}
