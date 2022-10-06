#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define MAX 80
#define PORT 5000
#define SA struct sockaddr
/* You will to add includes here */
#include <unistd.h>

// Included to get the support library
#include <calcLib.h>

// Enable if you want debugging to be printed, see examble below.
// Alternative, pass CFLAGS=-DDEBUG to make, make CFLAGS=-DDEBUG
#define DEBUG
void recieveMessage(int &socket_desc, char* server_message, unsigned int msg_size){

  //Clears the message array before recieving the server msg.
  //Because for some reason the function doesnt do it by itself
  memset(server_message, 0, msg_size);
  if(recv(socket_desc, server_message, msg_size, 0) < 0){
  	#ifdef DEBUG
  	printf("Error receiving message\n");
  	#endif
  	exit(-1);
  }
  else 
    printf(server_message);
} 
char* calculateMessage(char* server_message){
	int i1, i2, iresult;
	float f1, f2, fresult;
	char* operation, *grab1, *grab2, *temp1, *temp2;
	
	if(server_message[0] == 'f'){
		operation = strtok(server_message, " ");
		grab1 = strtok(NULL, " ");
		grab2 = strtok(NULL, "\\");
		f1 = atof(grab1); 
		f2 = atof(grab2);
		
		printf("Recieved: %s %8.8d %8.8d\n", operation, f1, f2);
		if(strcmp(operation, "fadd") == 0){
			fresult = f1+f2;
		} else if(strcmp(operation, "fsub") == 0){
			fresult = f1-f2;
		} else if(strcmp(operation, "fmul") == 0){
			fresult = f1*f2;
		} else if(strcmp(operation, "fdiv") == 0){
			fresult = f1/f2;
		}
		
		char* str = (char*)malloc(1450);
		sprintf(str, "%8.8g\n", fresult);
		return str;
		
	} else {
	
	  operation = strtok(server_message, " ");
		grab1 = strtok(NULL, " ");
		grab2 = strtok(NULL, "\\");
		i1 = atoi(grab1); 
		i2 = atoi(grab2);
		
		if(strcmp(operation, "add") == 0){
			iresult = i1+i2;
		} else if(strcmp(operation, "sub") == 0){
			iresult = i1-i2;
		} else if(strcmp(operation, "mul") == 0){
			iresult = i1*i2;
		} else if(strcmp(operation, "div") == 0){
		//Make result into string then add \n
			iresult = i1/i2;
		}
		char* str = (char*)malloc(1450);
		sprintf(str, "%d\n", iresult);
		return str;
	}


}
using namespace std;
int main(int argc, char *argv[]){
  
  /*
    Read first input, assumes <ip>:<port> syntax, convert into one string (Desthost) and one integer (port). 
     Atm, works only on dotted notation, i.e. IPv4 and DNS. IPv6 does not work if its using ':'. 
  */
  char delim[]=":";
  char *Desthost=strtok(argv[1],delim);
  char *Destport=strtok(NULL,delim);
  char server_message[2000], client_message[2000];
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, client;
  double fv1, fv2, fresult;
  int iv1, iv2, iresult;
  char msg[1450];
  // *Desthost now points to a sting holding whatever came before the delimiter, ':'.
  // *Dstport points to whatever string came after the delimiter. 
	
  /* Do magic */
  int port=atoi(Destport);
  #ifdef DEBUG  
  printf("Host %s, and port %d.\n",Desthost,port);
#endif

	//Create socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd == -1){
  	printf("Socket Creation Failed. Exiting...\n");
  	exit(-1);
  }
  else printf("Socket created successfully\n");
  
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(Desthost);
  servaddr.sin_port = htons(port);
  
  //Bind socket
  if((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0){
  	printf("Socket bind failed. Exiting...\n");
  	exit(-1);
  }
  else printf("Socket successfully bound\n");

	
	//Listen for connections
	//TODO 5 QUEUED CLIENTS
	if((listen(sockfd, 5)) != 0){
		printf("Failed to listen. Exiting...\n");
		exit(-1);
	}
	else printf("Listening...\n");
	
	while(true){
	//Accept connection, then send protocol msg
	connfd = accept(sockfd, (struct sockaddr*)&client, (socklen_t*)&len);
	
	if(connfd < 0){
		printf("Server accept failed. Exiting...\n");
		//exit(-1);
	}
	else {
		printf("Server accepted client.\n");
		char* str = "TEXT TCP 1.0\n\n";
		if(send(connfd, str, strlen(str), 0) < 0){
			printf("Could not send protocol msg\n");
			//exit(-1);
		} else printf("Sent protocol msg\n");
	}
	
	//Recv the OK from client
	recieveMessage(connfd, client_message, sizeof(client_message));
	
	//Compare strings to verify version
  if(strcmp(client_message,"OK\n") == 0){
  	#ifdef DEBUG
  	printf("Same\n");
  	#endif DEBUG
  	char* str = "OK\n";
  }
  //else return -1;
	
	
	//Send problem
	char* op = randomType();
	memset(msg, sizeof(msg), 0);
	
	if(op[0] == 'f'){
		fv1 = randomFloat();
		fv2 = randomFloat();
		sprintf(msg, "%s %8.8g %8.8g\n", op, fv1, fv2);
	}
	else{
		iv1 = randomInt();
		iv2 = randomInt();
		sprintf(msg, "%s %d %d\n", op, iv1, iv2);
	}
	if(send(connfd, msg, strlen(msg), 0) < 0){
			printf("Could not send protocol msg\n");
			//exit(-1);
	} else printf("Sent problem msg\n");
	
	//Recieve solution
	recieveMessage(connfd, client_message, sizeof(client_message));
	
	//Compare solutions
	char* solution;
	solution = calculateMessage(client_message);
	//Send final message
	}
}
