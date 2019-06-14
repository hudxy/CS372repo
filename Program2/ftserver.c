// Name: Hudson Dean
// Program Name: Client Server simple file transfer
// Description: Program uses socket programming to connect a client to a server to enter commands. The client will be able to use the following commands: -l, -g
//                -l command will allow client to view directory of server while -g will check if a filename associated with command exists, if it does, it will send contents of file to client.
//
// Course Name: CS372 Intro to Computer Networks
// Last Modified: 5/29/19

//Citations: Sockets Tutorial by Robert Ingalls      http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>

#define MAX 1024

//Function Description: Check command sent from client to see if what command was sent
//Pre-conditions: must be called within TCP connection, Client must have sent command to server
//Post-conditions: Returns value of 1 if client enters -l command
//                 Returns value of 2 if client enters -g command
//                 Returns value of 0 if client enters no matching command
int checkCmd(char * buff) {
  int i = 0;
  while(buff[i] != '\n') {
    if((char)buff[i] == '-') {
      if(buff[i+1] == 'g') {
        return 2;
      }
      if(buff[i+1] == 'l') {
        return 1;
      }
    }
    i++;
  }
  return 0;
}

//Function Description: Check command sent from client to see if what data port the data should be sent back on
//Pre-conditions: must be called within TCP connection, Client must have sent command to server, Data port must be last argument
//Post-conditions: Returns interger specifying the data port that the client expects data to be sent through
int getDataPort(char* buff) {
  char temp[10];
  memset(temp, '\0', sizeof(temp));
  //get end of command index
  int end = strlen(buff);
  //skip from end of command to get the last enter numbers from command
  while(!isdigit(buff[end])) {
    end--;
  }
  int count = 0;
  int start = end;
  //start counter number of digits for data port
  while(isdigit(buff[start])) {
    count++;
    start--;
  }
  //input numbers from dataport in command to a temp variable
  int i;
  for(i = 0; i<=count; i++) {
    temp[i] = buff[start];
    start++;
  }
  //convert str numbers to int numbers
  int dataPort = atoi(temp);
  //return that int value
  return dataPort;
}

//Function Description: Sends current directory information over TCP connection
//Pre-conditions: must be called within TCP connection, Client must have sent command to server
//Post-conditions: Writes to client the information of the current directory
void printDir(int sockfd) {
  char temp[MAX];
  DIR *d;
  struct dirent *dir;
  //open current directory
  d = opendir(".");
  if(d) {
    //write directory entries to tcp connection
    while((dir = readdir(d)) != NULL) {
      memset(temp, '\0', sizeof(temp));
      strcpy(temp, dir->d_name);
      write(sockfd, temp, sizeof(temp));
    }
  }
  return;
}

//Function Description: Checks file name given from command and sends contents of file over tcp connection
//Pre-conditions: must be called within TCP connection, Client must have sent command to server
//Post-conditions: Specific file contents are sent to client or Error message indicating that the file does not exist is sent to client
void requestFile(int sockfd, char* buff) {
  int i = 0;
  int count =0;
  char temp[MAX];
  memset(temp, '\0', sizeof(temp));
  //get position of file name
  while(buff[i] != '\n') {
    if(buff[i] == '-' && buff[i+1] == 'g') {
      count = i+3;
      break;
    }
    i++;
  }
  //get filename and set it to temp variable
  i=0;
  while(buff[count] != ' ') {
    temp[i] = buff[count];
    i++;
    count++;
  }
  //check if file name is in directory
  int fileExist = 0;
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if(d) {
    while((dir = readdir(d)) != NULL) {
      if(strcmp(temp, dir->d_name) == 0) {
        fileExist = 1;
      }
    }
  }
  //if file exists, send contents of file to client
  if(fileExist) {
    char fileName[MAX];
    strcpy(fileName, temp);
    FILE *fp;
    fp = fopen(fileName, "r");
    //error if file did not open
    if(!fp) {
      printf("ERROR opening file to send to client...\n");
      exit(1);
    }
    memset(temp, '\0', sizeof(temp));
    int length = 0;
    while(fgets(temp, MAX, fp) != NULL) {
        length = strlen(temp);
        write(sockfd, temp, length);
    }
    //close file
    close(fp);
  }
  //if file doesnt exist, send error message to client
  else{
    memset(temp, '\0', sizeof(temp));
    strcat(temp, "File not found...\n");
    write(sockfd, temp, 20);
  }
}

//Function Description: Recieves and checks command, sets-up new TCP connection to send data through
//Pre-conditions: must be called within TCP connection
//Post-conditions: Data is sent to client or error message indicating the command is not valid
void cmd(int sockfd, char* clienIP) {
  char buffer[MAX];
  memset(buffer, '\0', sizeof(buffer));
  if((read(sockfd, buffer, sizeof(buffer))) == -1) {
    perror("Error with reading command..");
    exit(1);
  }

  int cmdNum = -5;
  cmdNum = checkCmd(buffer);

  if(cmdNum) {
      //Set up TCP connection for data
      int dataPort = getDataPort(buffer);
      int qSockfd = socket(AF_INET, SOCK_STREAM, 0);
      if(qSockfd == -1) {
        printf("Error with creating data socket..\n");
        exit(2);
      }

      struct sockaddr_in servaddr1;
      bzero(&servaddr1, sizeof(servaddr1));

      servaddr1.sin_family = AF_INET;
      servaddr1.sin_port = htons(dataPort);
      if(inet_pton(AF_INET, clienIP, &servaddr1.sin_addr)<=0)
      {
       printf("\nInvalid address/ Address not supported \n");
       exit(1);
      }
      // connect the client socket to server socket
      if(connect(qSockfd, (struct sockaddr*)&servaddr1, sizeof(servaddr1)) < 0) {
       printf("Connection FROM the server failed...\n");
       exit(0);
      }
      //check which command was sent, call corresponding function
      if(cmdNum == 1) {
        printDir(qSockfd);
      }
      else if(cmdNum == 2) {
        requestFile(qSockfd, buffer);
      }
      //close Data TCP connection
      close(qSockfd);
      //send error indicating that no valid command was given
    } else {
      bzero(buffer, MAX);
      strcat(buffer, "Only the following commands are accepted: -g, -l... Please try again.\n");
      write(sockfd, buffer, 50);
    }
}


//Main Function
int main(int argc, char *argv[]) {
  int port;
  //Check arguments
  if(argc < 2) {
    fprintf(stderr, "Please eneter %s with a port number.\n", argv[0]);
    exit(1);
  }
  //Grab port #
  port = atoi(argv[1]);

  //Create socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd == -1) {
    printf("Error with creating socket..\n");
    exit(2);
  }

  //Create sockket address and assign IP & Port#
  struct sockaddr_in servaddr, clien;
  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  //Bind socket
  if((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
    printf("Error with socket binding..\n");
    exit(3);
  }

  //Listen
  if((listen(sockfd, 10)) != 0) {
    printf("Listening failed...\n");
    exit(4);
  } else {
    printf("%s listening on port number: %d\n", argv[0], port);
  }

  //accept incoming connections
  char clienIP[INET_ADDRSTRLEN];
  int cliLen = sizeof(clien);
  int confd;
  while(confd = accept(sockfd, (struct sockaddr*)&clien, &cliLen)) {
    //print error if connection fails
    if(confd < 0) {
      printf("Error with server accept\n");
      exit(5);
    } else {
      //get client IP
      struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&clien;
      struct in_addr ipAddr = pV4Addr->sin_addr;
      inet_ntop( AF_INET, &ipAddr, clienIP, INET_ADDRSTRLEN );
      cmd(confd, clienIP);
    }
  }

  //close connection
  close(confd);
  return 0;
}
