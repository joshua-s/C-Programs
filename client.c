/* An imporvement to a simple server/client in the internet domain using TCP
   The port number is passed as an argument
    *Improvement maintains up to 3 conversations *
CSIT231_SP23 - R. Myers
code by: Joshua Sepulveda & Brandon Bernard-Mendez
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, wroteit, readit;
    int n_chitter;

    struct sockaddr_in serv_addr;
    struct hostent *server;

/* This part of the code ensures that there is a port number argv[2] and
 * IP address/hostname argv[1] entered on the command line
*/
    char buffer[256];
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);

/* a client also needs to set up their side of the socket
 * this uses the "sockfd" call initiates the socket and opens the
 * ephemeral port used temporarily for the server to send responses
 * sockfd here is using the ephemeral port and the client IP adddress
 * gethostbyname resolves hostnames to IP addresses but does nothing
 * if an IP address is given
 * the "connect" command uses the information from the command line to
 * access the server and port we intend to connect to
*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,
          (char *) &serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

/* if the client has made it here "connect" has done its job by using
 * its side of the socket to connect to the servers side of the socket
 * and can begin sending a message and receiving a response
*/

    while (1) {
        printf("\nPlease tell us your Chitter (max 140 chars): ");
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);

        while(strlen(buffer) > 141){ //asks again for chitter if exceeds 140 characters
            printf("Chitter exceeds 140 characters. Please tell us your Chitter again (140 characters max): ");
            bzero(buffer, 256);
            fgets(buffer, 255, stdin);
        }

        if(strncasecmp(buffer, "bye", 3) == 0){ //closes client and server if bye is sent
            printf("Goodbye!\n");
            break;
        }

        wroteit = write(sockfd, buffer, strlen(buffer));
        if (wroteit < 0)
            error("ERROR writing to socket");

        bzero(buffer, 256);
        readit = read(sockfd, buffer, 255);
        if(readit < 0)
            error("ERROR reading from socket");

        printf("%s\n", buffer);

        char confirm[10];
        printf("Do you really want to send that chitter? (yes/no): ");//asks if chitter wants to be sent, if yes it is sent to the server and saved in the log
        fgets(confirm, sizeof(confirm), stdin);

        wroteit = write(sockfd, confirm, strlen(confirm));
        if(wroteit < 0)
            error("ERROR writing to socket");

        if (strncasecmp(confirm, "yes", 3) == 0) { //if answer is yes sent to server and saved
            bzero(buffer, 256);
            readit = read(sockfd, buffer, 255);
            if(readit < 0)
                error("ERROR reading from socket");
            printf("%s\n", buffer);
        }
        else {
            printf("Chitter not sent. \n"); //message is not sent to server to be saved in log
        }

        n_chitter++; //increment for amount of chitters
        if(n_chitter == 3)
            break;//ends client if chitters reach an amount of 3
        }
    close(sockfd);
    return 0;
}

