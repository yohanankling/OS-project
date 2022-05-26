//
// Created by yohanan on 15/05/22.
//
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
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
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

        string str = "PUSH hi, this is a test msg from client1!";
        if (send(sockfd, str.data(), str.size(), 0) == -1) {
            perror("send");
            close(sockfd);
        }
    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
        sleep(7);
        str = "POP";
        if (send(sockfd, str.data(), str.size(), 0) == -1) {
            perror("send");
            close(sockfd);
        }
        if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        buf[numbytes] = '\0';
        printf("client: received '%s'",buf);

    printf("\n and....explanation of what was happening and what we were testing: \n"
           "we were sending a message with client1 and tested the PUSH func was working, \n"
           "we tried to use TOP function with the 'test2' client to see if the stack memory is 'shared', \n"
           "after we POPed this client message with 'test2' user, \n (and use our func 'quit' to quiet quit without error..)\n"
           "finally we tried to get POP with this client and see that the stack is empty "
           "so we can assume the stack is shared and the function is work with correct input \n"
           "(as we were told)\n"
           "(we tested QUIT and empty stack function as well...\n");
    str = "QUIT";
    if (send(sockfd, str.data(), str.size(), 0) == -1) {
        perror("send");
        close(sockfd);
    }
        close(sockfd);
    return 0;
}
