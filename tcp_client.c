//
//  main.c
//  TCP_client
//
//  Created by Xiao WU on 2019-07-14.
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
#include <time.h>


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Stroing start time
    clock_t start_time = clock();

    // looping till required time is not acheived
    while (clock() < start_time + milli_seconds);
}

int main(void) {

    int clientsocket, con_out, sa_port,msg_count;
    struct sockaddr_in serveraddr;
    char buf[1024], sa_ip[16],str_num[6];

    //scoket
    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientsocket < 0)
    {
        error("Error in connection.\n");
    }
    printf("Client socket is created.\n");

    //connect to server
    printf("Please enter the server IP address.\n");
    scanf("%s", &sa_ip);
    printf("Please enter the server port number.\n");
    scanf("%d", &sa_port);
    memset(&serveraddr, '\0', sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(sa_ip);
    serveraddr.sin_port = htons(sa_port);
    con_out=connect(clientsocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if(con_out<0)
    {
        error("Error in connecting.\n");
    }
    // print server IP
    printf("client: server address is %s . Port number is %d.\n",inet_ntoa(serveraddr.sin_addr),sa_port);

    //initialize massage count
    msg_count=0;
    while(1)
    {
        sprintf(str_num, "%d", msg_count);
        send(clientsocket, &str_num, sizeof(msg_count), 0);
        msg_count++;
        delay(500);

        if(recv(clientsocket, buf, 1024, 0) < 0){
            printf("Error in receiving data.\n");
        }else{
            printf("Server: \t%s\n", buf);
        }
    }

    return 0;
}
