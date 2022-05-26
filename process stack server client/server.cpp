//
// Created by yohanan on 16/05/22.
//
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <iostream>
#include "stack.hpp"
#include <sys/mman.h>

#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10   // how many pending connections queue will hold


Stack *stack;

string convertToString(const char* a)
{
    string s = a;
    return s;
}
void chiledFunc(int new_fd){
    char data[1024];
    while (true) {
        memset(data, 0, 1024);
        if (recv(new_fd, data, 1024, 0) == -1) {
            perror("recv");
            exit(1);
        }
        cout << "recived : " << data << endl;
        string str = convertToString(data);
        string word;
        if (str.rfind("PUSH ", 0) == 0) {
            for (int i = 5; i < str.size(); ++i) {
                word += str[i];
            }
            if (!word.empty()) {
                stack->Push(word);
                if (send(new_fd, "your message were pushed", 24, 0) == -1) {
                    perror("send");
                    close(new_fd);
                }
            } else {
                if (send(new_fd, "you tried to insert an empty word...", 36, 0) == -1) {
                    perror("send");
                    close(new_fd);
                }
            }
        } else if (str.rfind("POP", 0) == 0) {
            string ans = stack->Pop();
            char *char_arr;
            char_arr = &ans[0];
            if (send(new_fd, char_arr, ans.size(), 0) == -1) {
                perror("send");
                close(new_fd);
            }
        } else if (str.rfind("TOP", 0) == 0) {
            string ans = "OUTPUT: " + stack->Top();
            char *char_arr;
            char_arr = &ans[0];
            if (send(new_fd, char_arr, ans.size(), 0) == -1) {
                perror("send");
                close(new_fd);
            }
        }
        else if (str.rfind("QUIT", 0) == 0) {
            if (send(new_fd, "QUIT", 4, 0) == -1) {
                perror("send");
            }
            cout << "client were disconnected" << endl;
            close(new_fd);
        }
        else {
            if (send(new_fd, "ERROR: <Invalid operation>", 26, 0) == -1) {
                perror("send");
                close(new_fd);
            }
        }
    }
}

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    //share the stack memory
    stack = (Stack*)mmap(nullptr, sizeof(Stack), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    //share the stack address
    char *address = (char*)mmap(nullptr, sizeof(struct Node)*1000, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    stack->address = address;

    printf("server: waiting for connections...\n");
    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);

        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
            chiledFunc(new_fd);
        }
    }
    return 0;
}