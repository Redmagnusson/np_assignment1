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
// Included to get the support library
#include <calcLib.h>

// Enable if you want debugging to be printed, see examble below.
// Alternative, pass CFLAGS=-DDEBUG to make, make CFLAGS=-DDEBUG
#define DEBUG
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
int main(int argc, char *argv[]){
  
  /*
    Read first input, assumes <ip>:<port> syntax, convert into one string (Desthost) and one integer (port). 
     Atm, works only on dotted notation, i.e. IPv4 and DNS. IPv6 does not work if its using ':'. 
  */
  char delim[]=":";
  char *Desthost=strtok(argv[1],delim);
  char *Destport=strtok(NULL,delim);
  char server_message[2000], client_message[2000];
  memset(client_message, 0, 2000);
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, client;
  double fv1, fv2, fresult;
  int iv1, iv2, iresult;
  char msg[1450];
  int readSize;
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
		//exit(-1);
	}
	else printf("Listening...\n");
	
	while(true){
	
	printf("\n\n\n\nNEW SOCKET CONNECTING\n");
	//Accept connection, then send protocol msg
	connfd = accept(sockfd, (struct sockaddr*)&client, (socklen_t*)&len);
	
	if(connfd < 0){
		printf("Server accept failed. Closing...\n");
		close(connfd);
		//exit(-1);
	}
	else {
		printf("Server accepted client.\n");
		char* str = "TEXT TCP 1.0\n\n";
		if(send(connfd, str, strlen(str), 0) < 0){
			printf("Could not send protocol msg\n");
			close(connfd);
		} else printf("Sent protocol msg\n");
	}
	
	//Recv the OK from client
	//recieveMessage(connfd, client_message, sizeof(client_message));

	readSize = recv(connfd, &client_message, sizeof(client_message), 0);
	if(readSize == 0){
		printf("Connection with socket died.\n");
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
  		printf("Wrong response. Closing connection...");
  		close(connfd);
  		continue;
  	}
	
	
	//Calculate problem
	char* op = (char*)malloc(1450);
	op = randomType();
	printf("Got randomtype: %s\n", op);
	char* solution = (char*)malloc(1450);
	if(strcmp("fadd", op) == 0){
		fv1 = randomFloat();
		fv2 = randomFloat();
		fresult = fv1 + fv2;
		sprintf(msg, "%s %8.8g %8.8g\n", op, fv1, fv2);
		sprintf(solution, "%8.8g\n", fresult);
	}
	else if(strcmp("fsub", op) == 0){
		printf("test\n");
		fv1 = randomFloat();
		fv2 = randomFloat();
		fresult = fv1 - fv2;
		sprintf(msg, "%s %8.8g %8.8g\n", op, fv1, fv2);
		printf("sprintf worked\n");
		sprintf(solution, "%8.8g\n", fresult);

	}
	else if(strcmp("fmul", op) == 0){
		printf("test\n");
		fv1 = randomFloat();
		fv2 = randomFloat();
		fresult = fv1 * fv2;
		sprintf(msg, "%s %8.8g %8.8g\n", op, fv1, fv2);
		sprintf(solution, "%8.8g\n", fresult);
	}
	else if(strcmp("fdiv", op) == 0){
		printf("test\n");
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
	printf("Problem made: %s\n", msg);
	printf("Solution made: %s\n", solution);
	//Send problem
	if(send(connfd, msg, strlen(msg), 0) < 0){
			printf("Could not send problem msg\n");
			close(connfd);
			continue;
	}else printf("Problem sent\n");

	//Recieve solution
	readSize = recv(connfd, &client_message, sizeof(client_message), 0);
	if(readSize == 0){
		printf("Connection with socket died.\n");
		close(connfd);
		continue;
	}
	client_message[readSize] = 0;
	
	//Compare solutions
	printf("Answer: %s\n Solution:%s\n", client_message, solution);
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
		printf("Could not send final response msg\n");
		close(connfd);
		continue;
	}else printf("Final response sent: %s\n", solution);
	//Close connection
	close(connfd);
	
	}//This is the while loop, dont touch
	
	close(sockfd);
}
