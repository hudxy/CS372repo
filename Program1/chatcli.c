// Name: Hudson Dean
// Program Name: Simple Chat Server
// Description: Program uses socket programming to connect a client to a server to intiate chat. Server is written in Python while client is written using C
//               Program intiates TCP connection between client and server until either host writes "/quit". Hosts alternate turns sending messages with Client going first.
// Course Name: CS372 Intro to Computer Networks
// Last Modified: 5/1/19

//Citations: Sockets Tutorial by Robert Ingalls      http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//prototypes
void sendData(char* buffer, int n, int sockfd);
void recieveData(char* buffer, int n, int sockfd);
int startUp(int sockfd, struct sockaddr_in serv_addr, struct hostent *server, int port);

int main(int argc, char *argv[]) {
  //intiate socket variables
  int sockfd, port, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  //buffer for recieved and sent data
  char buffer[701];
  //Error if user doesn't provide hostname or port number
  if(argc < 3) {
    fprintf(stderr, "Please enter: %s  with hostname and port\n", argv[0]);
    exit(1);
  }
  port = atoi(argv[2]);
  //get hostname and print error if no host exists
  server = gethostbyname(argv[1]);
  if(server == NULL) {
    printf("Host does not exist...\n");
    exit(1);
  }

  //start up socket connection
  sockfd = startUp(sockfd, serv_addr, server, port);

  //While connected
  while(1) {
    sendData(buffer, n, sockfd);
    //if client quits, disconnect
    int i = strncmp("/quit", buffer, 5);
    if(i == 0) {
      break;
    }
    recieveData(buffer, n, sockfd);
    //if server quits, disconnect
    i = strncmp("/quit", buffer, 5);
    if(i == 0) {
      break;
    }
  }

  close(sockfd);
  return 0;
}

//Function Description: Sends data to server, clears buffer and writes buffer to server
//Pre-conditions: must be called within TCP connection, Client must send message
//Post-conditions: Client message is sent to server, if error in writing occurs, error will be displayed program exits
void sendData(char* buffer, int n, int sockfd) {
  //clear buffer
  bzero(buffer , 700);
  //allow client to input message
  fgets(buffer, 700 , stdin);
  //sent client message to server, if error display error
  char bufferHandle[701] = "Client1: ";
  strcat(bufferHandle, buffer);
  n = write(sockfd , bufferHandle, strlen(bufferHandle));
  if(n < 0) {
    printf("Error On Writing..\n");
    exit(1);
  }
}

//Function Description: Recieves data from server, clears buffer and reads to buffer from server
//Pre-conditions: must be called within TCP connection, server must send message
//Post-conditions: Server message is displayed on client console, if error in reading occurs, error will be displayed program exits
void recieveData(char* buffer, int n, int sockfd) {
  //clear buffer
  bzero(buffer, 700);
  //read from server input, if error display error
  n = read(sockfd, buffer, 700);
  if(n < 0) {
    printf("ERROR on reading..\n");
    exit(1);
  }
  //show handle of server
  printf("SERVER: %s\n", buffer);
}

//Function Description: Intiates socket connection
//Pre-conditions: Port Number and Hostname must be provided from command arguments and are valid
//Post-conditions: TCP connection is established or Error is displayed that TCP connection failed and program exits
int startUp(int sockfd, struct sockaddr_in serv_addr, struct hostent *server, int port) {
  //intiate socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  //error if socket does not open
  if (sockfd < 0) {
    printf("ERROR opening socket\n");
    exit(1);
  }

  //Zero out memory
  bzero((char*) &serv_addr , sizeof(serv_addr));
  //set address family for TCP connection
  serv_addr.sin_family = AF_INET;
  //Copy server IP address for sever address variable
  bcopy((char *) server->h_addr, (char*) &serv_addr.sin_addr.s_addr , server->h_length);
  //Convert host port number into network byte order using htons()
  serv_addr.sin_port = htons(port);

  //Connect to server, if failed: display error
  if(connect(sockfd , (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("CONNECTION FAILED...\n");
    exit(1);
  }
  return sockfd;
}
