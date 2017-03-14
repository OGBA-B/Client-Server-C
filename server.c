//
//  server.c
//  Lab
//
//  Created by bobbie Okparanyote on 2017-01-05.
//  Copyright © 2017 bobbie Okparanyote. All rights reserved.
//

/*TCP SERVER*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define MAX_CLIENTS 5
#define BUFFER_SIZE 256
typedef struct sockaddr SOCKADDR;
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portNumber;
    socklen_t clilen;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2) // If no port Number was entered
    {
        fprintf(stderr,"ERROR: no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portNumber = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portNumber);
    if (bind(sockfd, (SOCKADDR *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR: Could not bind");
    listen(sockfd,MAX_CLIENTS);
    clilen = sizeof(cli_addr);
    int pid;
    while (1) {
    newsockfd = accept(sockfd,(SOCKADDR *) &cli_addr,&clilen);
        
        if (newsockfd < 0)
        error("ERROR on accept");
    /*Handle multiple clients in parallel*/
    pid = fork();
    if (pid < 0)
    {
        error("ERROR: New process could not be created");
    }
    if(pid==0)// child
    {
    close(sockfd);
    memset(buffer, 0, sizeof(buffer));
    n = read(newsockfd,buffer,BUFFER_SIZE-1);
    if (n < 0)
    {
        error("ERROR reading from socket");
    }
    printf("From client(%s): %s\n",inet_ntoa(cli_addr.sin_addr),buffer);
    n = write(newsockfd,"Server Received Message",23);
        if(n < 0)
        {
            
           error("ERROR writing to socket");
        }
        exit(0);
    }
    else{//parent
        close(newsockfd);
    }
}
    close(sockfd);
    return 0;
}