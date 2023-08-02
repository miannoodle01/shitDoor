#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

int port1;
void helpFunction();

int main(int argc, char *argv[]) {
    
    if ((argc == 2) && (argv[1] != NULL) && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        helpFunction();
        exit(EXIT_SUCCESS);
    }
    
    if (argc < 2) {
        perror("Missing argument, for usage see ./a.out --help or ./a.out -h");
            exit(EXIT_FAILURE);
    }
    
    int sockfd1;
    int sockfd2;
    int port1;
    int bind1;
    int listen1;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    socklen_t clientLenght;
    char receiveBuffer[1024];
    char sendBuffer[1024];
    char *serverIp;
    pid_t cpid;
    bzero(&serverAddress, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverIp = argv[1];
    serverAddress.sin_addr.s_addr = inet_addr(serverIp);
    port1 = atoi(argv[2]);
    serverAddress.sin_port = htons(port1);

    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 < 0) {
        perror("Error occured on creating socket ( socket() function error )");
        exit(EXIT_FAILURE);
    } else if (sockfd1 > 0) {
        printf("Successfully connected the socket ( socket() ).\n");
    }

    bind1 = bind(sockfd1, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if (bind1 < 0 ) {
        perror("Error occured on binding socket");
        exit(EXIT_FAILURE);
    } else if (bind1 == 0) {
        printf("Successfully binded socket to eachother ( bind() ).\n");
    }
 
    listen1 = listen(sockfd1, 5);
    if (listen1 < 0) {
        perror("Error occured on listening on socket");
        exit(EXIT_FAILURE);
    } else if (listen1 == 0) {
        printf("Successfully listening on socket ( socket() ).\n");
    }

    printf("Your shit server is running.\n");

    while (1) {
        sockfd2 = accept(sockfd1, (struct sockaddr *) &clientAddress, &clientLenght);
        if (sockfd2 < 0) {
            perror("Error occured on accepting data from socket");
            exit(EXIT_FAILURE);
        } else {
            printf("Successfully extracted the first connection request\n");
            printf("on the queue of pending connections for the listening socket ( accept() ).\n");
        }

        sleep(3);

        while (1) {
            bzero(&receiveBuffer, sizeof(receiveBuffer));
            if (recv(sockfd2, receiveBuffer, sizeof(receiveBuffer), 0) < 0) {
                perror("Error occured on receiving data from socket");
                close(sockfd1);
                exit(EXIT_FAILURE);
            }
            
            printf("Enter command to execute: ");
            fgets(sendBuffer, 1023, stdin);
            if (send(sockfd1, sendBuffer, sizeof(sendBuffer), 0) < 0) {
                perror("Error occured on sending data to socket");
                close(sockfd1);
                exit(EXIT_FAILURE);
            }
        }
        cpid = fork();
        if (cpid == 0) {
            while(1) {
                if (receiveBuffer == NULL) {
                    printf("empty receive buffer\n");
                } else if (receiveBuffer != NULL) {
                    printf("buffer is not empty\n");
                }
                if (recv(sockfd2, receiveBuffer, sizeof(receiveBuffer), 0) < 0) {
                    perror("Error occured on receiving data from socket");
                    exit(EXIT_FAILURE);
                }
                printf("client: %s\n", receiveBuffer);
                bzero(&receiveBuffer, sizeof(receiveBuffer));
                sleep(1);
            }
        } else {
            while(1) {
                printf("Enter command: ");
                fgets(sendBuffer, 1023, stdin);
                sleep(3);
                if (send(sockfd2, sendBuffer, strlen(sendBuffer) + 1, 0) < 0) {
                    perror("Error occured on sending data to socket");
                    exit(EXIT_FAILURE);
                } else {
                    printf("Sending data to socket.\n");
                }
                bzero(&sendBuffer, sizeof(sendBuffer));
                sleep(1);
            }
        }   
    }
    return 0;
}

void helpFunction() {
    printf("help message");
}