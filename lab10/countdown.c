#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>    /* Internet domain header */
#include "wrapsock.h"

#define MAXLINE 1024
#ifndef PORT
#define PORT 10000
#endif
#define LISTENQ 10



int main(int argc, char **argv) {
    int  maxfd, listenfd, connfd;
    int nready, counter;
    fd_set	rset, allset;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    int yes = 1;
    struct timeval timeout;
    fprintf(stderr, "Port = %d\n", PORT);

    listenfd = Socket(PF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = PF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(PORT);

    if((setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) == -1) {
        perror("setsockopt");
    }

    Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    // We are only accepting 1 connection so it doesn't need to be in the
    // set for select.
    clilen = sizeof(cliaddr);
    connfd = Accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
    maxfd = connfd;	/* initialize */

    FD_ZERO(&allset);
    FD_SET(connfd, &allset);
    counter = 10;

    for ( ; ; ) {
        rset = allset;		/* structure assignment */
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        nready = Select(maxfd+1, &rset, NULL, NULL, &timeout);

        if(nready == 0) {
            counter--;
            printf("%d\n", counter);
            // Write the counter to the client.
            if(write(connfd, &counter, sizeof(int)) != sizeof(int)) {
                perror("write");
            }
            if(counter <= 0) {
                printf("Kaboom\n");
                exit(0);
            }
        }

        if (FD_ISSET(connfd, &rset)) {	
	    fprintf(stderr, "reading from client\n");
            if(read(connfd, &counter, sizeof(int)) != sizeof(int)) {
                fprintf(stderr, "Client closed or error\n");
                exit(0);
            } 
        }


    }
}
