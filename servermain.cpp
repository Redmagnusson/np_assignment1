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
#include <math.h>
#include <errno.h>
// Included to get the support library
#include <calcLib.h>
#define DEBUG
// Enable if you want debugging to be printed, see examble below.
// Alternative, pass CFLAGS=-DDEBUG to make, make CFLAGS=-DDEBUG
//#define DEBUG
void recieveMessage(int &socket_desc, char* server_message, unsigned int msg_size){

  //Clears the message array before recieving the server msg.
  //Because for some reason the function doesnt do it by itself
  memset(server_message, 0, 2000);
  if(recv(socket_desc, server_message, msg_size, 0) < 0){
  	#ifdef DEBUG
  	printf("Error receiving message\n");
  	#endif
  	exit(-1);
  }
  else 
    printf(server_message);
   
} 

using namespace std;
int CAP = 2000;
int main(int argc, char *argv[]){
  //Variables
	char* splits[CAP];
  char* p = strtok(argv[1], ":");
  int delimCounter = 0;
  char *Desthost;
  char *Destport;
  int port;
	int sockfd;
	int connfd;
	struct sockaddr_in client;
	char server_message[CAP];
	char client_message[CAP];
	char msg[CAP];
	int len = 0;
	float fv1, fv2, fresult;
	int iv1, iv2, iresult;
	int readSize;
	
  //Get argv
  while(p != NULL){
  	//Look for the amount of ":" in argv to determine if ipv4 or ipv6
  	splits[delimCounter++] = p;
  	p = strtok(NULL, ":");
  }
  Destport = splits[--delimCounter];
  Desthost = splits[0];
  for(int i = 1;i<delimCounter;i++){
  	
  	sprintf(Desthost, "%s:%s",Desthost, splits[i]);
  }
  port=atoi(Destport);

	
	//Getaddrinfo
	struct addrinfo hints, *serverinfo = 0;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	if(getaddrinfo(Desthost, Destport, &hints, &serverinfo) < 0){
		printf("Getaddrinfo error: %s\n", strerror(errno)); 
		exit(0);
	} else printf("Getaddrinfo success\n");

  printf("Host %s, port %d\n",Desthost,port);
  
  printf("IPV: %d\n", serverinfo->ai_family);
  
	//Create socket
  sockfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
  if(sockfd == -1){
  	#ifdef DEBUG
  	printf("Socket Creation Failed. Exiting...\n");
  	#endif
  	exit(-1);
  }
  else{
  	#ifdef DEBUG
  	printf("Socket created successfully\n");
  	#endif
  }
  
  //Set socket option for timeout
  struct timeval tv = {
  	.tv_sec = 5
  };
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  //Bind socket
  if((bind(sockfd, serverinfo->ai_addr, serverinfo->ai_addrlen)) < 0){
  	#ifdef DEBUG
  	printf("Socket bind failed. Exiting...\n");
  	#endif
  	exit(-1);
  }
  else {
  	#ifdef DEBUG
  	printf("Socket successfully bound\n");
  	#endif
  }

	
	//Listen for connections
	//TODO 5 QUEUED CLIENTS
	if((listen(sockfd, 5)) != 0){
		#ifdef DEBUG
		printf("Failed to listen. Exiting...\n");
		#endif
		exit(-1);
	}
	else {
		#ifdef DEBUG
		printf("Listening...\n");
		#endif
	}
	while(true){
	
	//printf("\n\n\n\nNEW SOCKET CONNECTING\n");
	//Accept connection, then send protocol msg
	connfd = accept(sockfd, (struct sockaddr*)&client, (socklen_t*)&len);
	
	if(connfd < 0){
		#ifdef DEBUG
		printf("Server accept timeout. Closing connection.\n");
		#endif
		close(connfd);
	}
	else if(connfd == 0){
		#ifdef DEBUG
		printf("Server accept timeout. Closing connection.\n");
		#endif
		close(connfd);
	}
	else {
		#ifdef DEBUG
		printf("Server accepted client.\n");
		#endif
		char* str = "TEXT TCP 1.0\n\n";
		if(send(connfd, str, strlen(str), 0) < 0){
			#ifdef DEBUG
			printf("Could not send protocol msg\n");
			#endif
			close(connfd);
		} else {
			#ifdef DEBUG
			printf("Sent protocol msg\n");
			#endif
		}
	}
	
	//Recv the OK from client
	//recieveMessage(connfd, client_message, sizeof(client_message));

	readSize = recv(connfd, &client_message, sizeof(client_message), 0);
	if(readSize < 0){
		#ifdef DEBUG
		printf("Connection with socket died.\n");
		#endif
		close(connfd);
		continue;
	}
	else if(readSize == 0){
		#ifdef DEBUG
		printf("Client timeout. Closing connection.\n");
		// ADD ERROR MESSAGE SEND HERE
		#endif
		close(connfd);
		continue;
	}
	client_message[readSize] = 0;
	//Compare strings to verify version
  	if(strcmp(client_message,"OK\n") == 0){
  		#ifdef DEBUG
  		printf("Same\n");
  		#endif DEBUG
  	}
  	else{
  		#ifdef DEBUG
  		printf("Wrong response. Closing connection...");
  		#endif
  		
  		close(connfd);
  		continue;
  	}
	
	
	//Calculate problem
	char* op = (char*)malloc(1450);
	op = randomType();
	char* solution = (char*)malloc(1450);
	if(strcmp("fadd", op) == 0){
		fv1 = randomFloat();
		fv2 = randomFloat();
		fresult = fv1 + fv2;
		sprintf(msg, "%s %8.8g %8.8g\n", op, fv1, fv2);
		sprintf(solution, "%8.8g\n", fresult);
	}
	else if(strcmp("fsub", op) == 0){
		fv1 = randomFloat();
		fv2 = randomFloat();
		fresult = fv1 - fv2;
		sprintf(msg, "%s %8.8g %8.8g\n", op, fv1, fv2);
		sprintf(solution, "%8.8g\n", fresult);

	}
	else if(strcmp("fmul", op) == 0){
		fv1 = randomFloat();
		fv2 = randomFloat();
		fresult = fv1 * fv2;
		sprintf(msg, "%s %8.8g %8.8g\n", op, fv1, fv2);
		sprintf(solution, "%8.8g\n", fresult);
	}
	else if(strcmp("fdiv", op) == 0){
		fv1 = randomFloat();
		fv2 = randomFloat();
		fresult = fv1 / fv2;
		sprintf(msg, "%s %8.8g %8.8g\n", op, fv1, fv2);
		sprintf(solution, "%8.8g\n", fresult);
	}
	else if(strcmp("add", op) == 0){
		iv1 = randomInt();
		iv2 = randomInt();
		iresult = iv1 + iv2;
		sprintf(msg, "%s %d %d\n", op, iv1, iv2);
		sprintf(solution, "%d\n", iresult);
	}
	else if(strcmp("sub", op) == 0){
		iv1 = randomInt();
		iv2 = randomInt();
		iresult = iv1 - iv2;
		sprintf(msg, "%s %d %d\n", op, iv1, iv2);
		sprintf(solution, "%d\n", iresult);
	}
	else if(strcmp("mul", op) == 0){
		iv1 = randomInt();
		iv2 = randomInt();
		iresult = iv1 * iv2;
		sprintf(msg, "%s %d %d\n", op, iv1, iv2);
		sprintf(solution, "%d\n", iresult);
	}
	else if(strcmp("div", op) == 0){
		iv1 = randomInt();
		iv2 = randomInt();
		iresult = iv1 / iv2;
		sprintf(msg, "%s %d %d\n", op, iv1, iv2);
		sprintf(solution, "%d\n", iresult);
	}
	
	//Send problem
	if(send(connfd, msg, strlen(msg), 0) < 0){
			#ifdef DEBUG
			printf("Could not send problem msg\n");
			#endif
			close(connfd);
			continue;
	}else{
		#ifdef DEBUG
		printf("Problem sent\n");
		#endif
		printf("%s", msg);
	}

	//Recieve solution
	readSize = recv(connfd, &client_message, sizeof(client_message), 0);
	if(readSize < 0){
		close(connfd);
		continue;
	}
	else if(readSize == 0){
		#ifdef DEBUG
		printf("Client timeout. Closing connection\n");
		#endif
		// ADD ERROR MESSAGE SEND HERE
		close(connfd);
		continue;
	}
	
	printf("%s", client_message);
	client_message[readSize] = 0;
	
	//Compare solutions
	if(op[0] == 'f'){
		double x = fabs(atof(client_message) - atof(solution));
		if(x < 0.0001){
			solution = "OK\n";
		}
		else solution = "ERROR\n";
	}
	else if(strcmp(client_message, solution) == 0){
		solution = "OK\n";
	}else{
		solution = "ERROR\n";
	}
	
	//Send final response
	if(send(connfd, solution, strlen(solution), 0) < 0){
		#ifdef DEBUG
		printf("Could not send final response msg\n");
		#endif
		close(connfd);
		continue;
	}else printf("%s\n", solution);
	//Close connection
	close(connfd);
	
	}//This is the while loop, dont touch
	
	close(sockfd);
}
