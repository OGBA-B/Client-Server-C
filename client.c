//
//  client.c
//  Lab
//
//  Created by bobbie Okparanyote on 2017-01-05.
//  Copyright © 2017 bobbie Okparanyote. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
void waitS(int seconds) //delay in seconds
{
    
    clock_t start= clock();
    int stop;
    stop=0;
    while(stop==0)
    {
        int value=(clock()-start)/CLOCKS_PER_SEC;
        
        if(value >=seconds) stop=1;
    }
}
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    char buffer[256];
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    int i=1;
    while(1){
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR: Could not open socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR: Host could not be found\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR: could not connect");
   
    bzero(buffer,256);
    //fgets(buffer,255,stdin);
    sprintf(buffer, "Message Number %d", i++);
    printf("Sent Message: %s\n", buffer);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
        error("ERROR: Could not write to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
        error("ERROR: Could not read from socket");
        printf("%s\n",buffer);
        waitS(5);// wait for 5 second before sending next message
    }
    close(sockfd);
    return 0;
}