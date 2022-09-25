#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define SA struct sockaddr
/* You will to add includes here */

// Enable if you want debugging to be printed, see examble below.
// Alternative, pass CFLAGS=-DDEBUG to make, make CFLAGS=-DDEBUG
#define DEBUG


// Included to get the support library
#include <calcLib.h>

int main(int argc, char *argv[]){

  /*
    Read first input, assumes <ip>:<port> syntax, convert into one string (Desthost) and one integer (port). 
     Atm, works only on dotted notation, i.e. IPv4 and DNS. IPv6 does not work if its using ':'. 
  */
  char delim[]=":";
  char *Desthost=strtok(argv[1],delim);
  char *Destport=strtok(NULL,delim);
  struct sockaddr_in servaddr;
  // *Desthost now points to a sting holding whatever came before the delimiter, ':'.
  // *Dstport points to whatever string came after the delimiter. 

  /* Do magic */
  int port=atoi(Destport);
  
  //Create TCP socket
  int socket_desc;
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_desc == -1){
  	printf("socket creation failed...\n");
  	exit(0);
  }
  else
  	printf("socket creation succeeded...\n");
  
  printf("starting server connection");
  bzero(&servaddr, sizeof(servaddr));
  
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(Desthost);
  servaddr.sin_port = 5000;
  
  if(connect(socket_desc, (SA*)&servaddr, sizeof(servaddr)) != 0) {
  	printf("connection with the server failed...\n");
  	exit(0);
  }
  else
  	printf("connected to server...\n");	
  
#ifdef DEBUG 
  printf("Host %s, and port %d.\n",Desthost,port);
#endif

  
}
