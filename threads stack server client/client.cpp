//
// Created by yohanan on 09/05/22.
//
/*
** client.c -- a stream socket client demo
*/

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
using namespace std;



#define PORT "3490" // the port client will be connecting to

#define MAXDATASIZE 2048 // max number of bytes we can get at once

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure


    // stack code:
    printf("\tMENU");
    printf("\n----------------------------\n");
    printf("      1. PUSH TEXT FOR EXAMPLE\n");
    printf("      2. POP\n");
    printf("      3. TOP\n");
    printf("      3. QUIT\n");
    printf("----------------------------\n");
    while (true) {
        printf("Choose operation : ");
        string str;
        getline(cin, str);
        if (send(sockfd, str.data(), str.size(), 0) == -1) {
            perror("send");
            close(sockfd);
        }
        if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        buf[numbytes] = '\0';
        string s(buf);
        if (s.rfind("OUTPUT: ", 0) == 0) {
            printf("%s\n",buf);
        } else if (s.rfind("ERROR: <", 0) == 0) {
            printf("%s\n",buf);
        }  else if (s.rfind("QUIT", 0) == 0) {
            printf("you'r disconnected\n");
            close(sockfd);
            return 0;
        }  else{
            printf("client: received '%s'\n",buf);}
//        close(sockfd);
    }
    return 0;
}
