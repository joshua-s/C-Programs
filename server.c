/* An imporvement to a simple server/client in the internet domain using TCP
   The port number is passed as an argument
    *Improvement maintains up to 3 conversations *
CSIT231_SP23 - R. Myers
code by: Joshua Sepulveda & Brandon Bernard-Mendez
*/
#include <signal.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>


void error(char *msg) {
    perror(msg);
    exit(1);
}


void chitter_log(const char *chitters){ //function creates log and appends date and time to all chitters
    FILE *logfile = fopen("chitters.txt", "a");
    if(logfile == NULL){
        perror("Error opening log file...");
        return;
    }
    struct timeval dt;
    gettimeofday(&dt, NULL);
    time_t actualtime = dt.tv_sec;
    struct tm *timeinfo = localtime(&actualtime);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

    if(strlen(chitters) > 0) {
        fprintf(logfile, "[%s] %s", timestamp, chitters);
    }
    else{
        char not_sent[] = "MESSAGE NOT SENT";
        fprintf(logfile, "[%s] %s", timestamp, not_sent);
    }
    fclose(logfile);
}

int main(int argc, char *argv[]) {

    int sockfd, newsockfd, portno, clilen, server_write;
    int chatter;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int readit;

    listen(sockfd, 5); //makes sure that the server stays up and listening
    clilen = sizeof(cli_addr);

    /*the following construct ensures that a port number was provided
     * on the command line
     */
    if (argc < 2) { //error handling if no port is entered on the command line
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    /* a socket is created here "sockfd" is the "socket file descriptor
     * into which messages are written and read
     * if sockfd is less than zero after running the socket command the
     * command failed
     * "serv_addr" is a built-in struct to socket.h that can contain the
     * port number of a socket (sin_port)
     * version of IP used (sin_family)
     * addresses permitted on which connections can be accepted (s_addr)
     * The "bzero" function is a part of "string.h" that zeros out the memory
     * space reserved for a variable/struct whose values will be set later
     */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    if(portno < 1023 || portno > 49151) { //makes sure that user enters a valid port number
        printf("Warning, this port number is either \"well-known\" or \"ephemeral\". ");
        printf("\nTry again. Enter a port number between 1023 and 49151 (inclusive).");
        exit(1);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* This portion of the code now binds the server address to the file
     * decriptor of the socket created above
     */
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    //gets the PID of the server and through system command line finds the port number and running program
    pid_t pid = getpid();
    char system_cmd[256];
    snprintf(system_cmd, sizeof(system_cmd),"ps -p %d -o args= | grep -oE 'server [0-9]+'", pid); //limits the output to only server + port number

    /* In this portion of the code the "listen" command is issued followed
     * by the "accept" command which will complete bringing up the server
     * since it can now listen for and accept new connections form clients
     */
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    /* in the final part of this code the server reads the message sent by the
     * client and responds back positively if it had received it.
     */

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");

        chatter = 0; //keeps count of the conversations.
        while (1) {
            bzero(buffer, 256);
            readit = read(newsockfd, buffer, 255);
            if (readit < 0)
                error("ERROR reading from socket");

            //chitter_log(buffer);

            system(system_cmd); //prints out the server + port number
            char response[512];
            strcpy(response, "Chatter: ");
            strcat(response, buffer);
            server_write = write(newsockfd, response, strlen(response));
            if(server_write < 0)
                error("ERROR writing to socket");

            bzero(buffer, 256);
            readit = read(newsockfd, buffer, 255);
            if(readit < 0)
                error("ERROR reading to socket.");


            if(strncasecmp(buffer, "yes", 3) == 0) { //sends the chatter into the log if the user inputs yes
                chitter_log(response + strlen("Chatter: "));
                char chattered[] = "Chattered!";
                server_write = write(newsockfd, chattered, strlen(chattered));
                if (server_write < 0)
                    error("ERROR writing to socket");
            }
            else{//placeholder in the log for messages that are not sent
                chitter_log("MESSAGE NOT SENT\n");
            }
            chatter++;
            if (chatter == 3) {
                break;
            }
        }
        close(newsockfd); // stays on even after client closes
        //return 0; this has to be removed in order for the server to stay up and listening for any client connection
    }
    close(sockfd);
    return 0;
}
