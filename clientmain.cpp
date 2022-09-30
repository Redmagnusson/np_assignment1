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
//Function to recieve a message from the server
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
void sendMessage(int &socket_desc, char* client_message, unsigned int msg_size){

  //Sends message to client, clearing the array at the end to not interfere with
  //the messages we put in before calling the function
  if(send(socket_desc, client_message, msg_size, 0) < 0){
	#ifdef DEBUG
  	printf("Unable to send message\n");
  	#endif DEBUG
  	exit(-1);
  }
  else printf(client_message);
  
  memset(client_message, 0, msg_size);
}
void calculateMessage(char* server_message, char* client_message){
	int i1, i2, iresult;
	float f1, f2, fresult;
	char* operation, *grab1, *grab2, *temp1, *temp2;
	
	if(server_message[0] == 'f'){
		operation = strtok(server_message, " ");
		grab1 = strtok(NULL, " ");
		grab2 = strtok(NULL, "\\");
		#ifdef DEBUG
		printf("%s %s %s", operation, grab1, grab2);
		#endif
		f1 = atof(grab1); 
		f2 = atof(grab2);
		
		if(strcmp(operation, "fadd") == 0){
			fresult = f1+f2;
		} else if(strcmp(operation, "fsub") == 0){
			fresult = f1-f2;
		} else if(strcmp(operation, "fmul") == 0){
			fresult = f1*f2;
		} else if(strcmp(operation, "fdiv") == 0){
			fresult = f1/f2;
		}


		sprintf(temp1, "%8.8g", fresult);
		strcpy(client_message, temp1);
		strcat(client_message, "\n");
		return;
		
	} else {
	
	  operation = strtok(server_message, " ");
		grab1 = strtok(NULL, " ");
		grab2 = strtok(NULL, "\\");
		#ifdef DEBUG
		printf("%s %s %s", operation, grab1, grab2);
		#endif
		i1 = atoi(grab1); 
		i2 = atoi(grab2);
		
		if(strcmp(operation, "add") == 0){
			iresult = i1+i2;
		} else if(strcmp(operation, "sub") == 0){
			iresult = i1-i2;
		} else if(strcmp(operation, "mul") == 0){
			iresult = i1*i2;
		} else if(strcmp(operation, "div") == 0){
			iresult = i1/i2;
		}
		#ifdef DEBUG
		printf("Result: %s", client_message);
		#endif
		sprintf(client_message, "%d", iresult);
		strncat(client_message, "\n", 2);

		return;
	}


}

int main(int argc, char *argv[]){

  char delim[]=":";
  char *Desthost=strtok(argv[1],delim);
  char *Destport=strtok(NULL,delim);
  struct sockaddr_in servaddr;
  char server_message[2000], client_message[2000];
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

  //Create Socket Structure
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(Desthost);
  int error;
  
  //Establish Connection
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
  
  //Recieve message from server
  recieveMessage(socket_desc, server_message, sizeof(server_message));
  
  //Compare strings to verify version
  if(strcmp(server_message,"TEXT TCP 1.0\n\n") == 0){
  	#ifdef DEBUG
  	printf("Same\n");
  	#endif DEBUG
  	char* str = "OK\n";
  	strcpy(client_message, str);
  }
  else return -1;

  //Send back the OK
  sendMessage(socket_desc, client_message, sizeof(client_message));
  
  //Recieve the problem
  recieveMessage(socket_desc, server_message, sizeof(server_message));
  
  //Translate Message
  memset(client_message, 0, sizeof(client_message));
  calculateMessage(server_message, client_message);
  printf(client_message);
	
  //Send answer to server
  sendMessage(socket_desc, client_message, sizeof(client_message));
  
  //Recieve the final Message
  recieveMessage(socket_desc, server_message, sizeof(server_message));
  
  //Close socket and quit program
  //TODO
}
