//
//  main.c
//  tcp_server
//
//  Created by Xiao Wu on 2019-07-14.
//  Copyright © 2019 Xiao WU. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#define server_port  8080
#define maxlength 1024

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main (void)
{
    struct sockaddr_in serveraddr, clientaddr;
    int serversocket, addrlen, clientsocket, sent_out,bind_out, recv_out;
    char buf [maxlength], loip[16];
    pid_t childpid;
    //socket
    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serversocket < 0)
    {
        error("Error in connection.\n");
    }
    printf("Server Socket is created.\n");

    //bind
    printf("Please enter you local IP address.\n");
    scanf("%s", &loip);
    memset(&serveraddr, '\0', sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(loip);
    serveraddr.sin_port = htons(server_port);
    bind_out=bind(serversocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if(bind_out<0)
    {
        error("Error in binding.\n");
    }
    // print server IP
    printf("server: server address is %s . Port number is %d.\n",inet_ntoa(serveraddr.sin_addr),server_port);

    //listen
    if(listen(serversocket, 20)==0)
    {
        printf("Listening...");
    }else{
        error("Error in listening\n");
    };

    while(1)
    {
        addrlen = sizeof(clientaddr);
        clientsocket = accept(serversocket, (struct sockaddr *)&clientaddr, &addrlen);
        if (clientsocket < 0)
        {
            error("Error in connecting client.\n");
        }
        printf("server: got connection from %s @ port %d.\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

        if((childpid = fork())==0)
        {

            while(1)
            {
                recv_out = recv(clientsocket, buf, maxlength,0);
                if((strcmp(buf, ":exit") == 0)||(recv_out == 0)){
                    printf("Disconnected from %s:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
                    break;
                }else{
                    printf("Client: %s\n", buf);
                    send(clientsocket, buf, strlen(buf), 0);
                    bzero(buf, sizeof(buf));
                }
            }
            close(clientsocket);
        }
    }
    return 0;
}
