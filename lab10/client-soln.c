/***** inetclient.c *****/ 
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>    
#include <netdb.h>
#include "wrapsock.h"

#ifndef PORT
#define PORT 30000
#endif

#define BUF_SIZE 256

int main(int argc, char* argv[])
{ 
    int sockfd;
    char buf[BUF_SIZE];
    struct hostent *hp;
    struct sockaddr_in server;
    fd_set allset, rset;
    int nready, maxfd;
    int nread;
    int server_counter;


    server.sin_family = PF_INET;
    server.sin_port = htons(PORT); 
    //bzero(&(server.sin_zero), 8);   /* zero the rest of the struct */
    printf("PORT = %d\n", PORT);


    if ( argc != 2 )
    {  
        fprintf(stderr, "Usage: %s hostname\n", argv[0]);
        exit(1);
    }

    /* fill in peer address */
    hp = gethostbyname(argv[1]);                
    if ( hp == NULL ) {  
        fprintf(stderr, "%s: %s unknown host\n",
            argv[0], argv[1]);
        exit(1);
    }

    server.sin_addr = *((struct in_addr *)hp->h_addr);

    /* PF_INET = IPv4 family protocol */
    sockfd = Socket(PF_INET, SOCK_STREAM, 0);
    Connect(sockfd, (struct sockaddr *)&server, sizeof(server));

    FD_ZERO(&allset);				/* set up the set of fds to monitor */
    FD_SET(sockfd, &allset);		/* set up the socket set to monitor */
    FD_SET(fileno(stdin), &allset);	/* set up the socket set to monitor */

    maxfd = sockfd;					/* set max FD to the first connection */
                                    /* because stdin is 0 */
    nready = maxfd+1;
    while(1) {
        rset = allset;

        // prompt user to type a number
        fprintf(stderr, "Enter a number: \n");
        
        // use select to multiplex between stdin and the socket.
        // timeout should be NULL

        // note how it is maxfd + 1, because select checks 
        // what you give - 1!
        if (select (nready, &rset, NULL, NULL, NULL) < 0) break;

        // if stdin bit is set, read from it using fgets.
        if (FD_ISSET (fileno(stdin), &rset)){
            if (!fgets(buf, BUF_SIZE, stdin)){
                fprintf(stderr, "Error reading from stdin");
                break;
            }
            
            // convert the string read into a number
            nread =  strtol(buf, NULL, 10);
            
            // write it to the socket
            if (write(sockfd, &nread, sizeof(int)) != sizeof(int)){
                fprintf(stderr, "Failed to write to socket");
                break;
            }

        }

        // if the socket is set
        if (FD_ISSET(sockfd, &rset)){
            // read to the number from the socket and print it
            if (read(sockfd, &server_counter, sizeof(int)) != sizeof(int)){
                fprintf(stderr, "Read Error from server, closing connection\n");
                break;
            }
            printf("Counter is %d\n", server_counter);
        }
    }   
    Close(sockfd); 
    return(0);
}

/* Print statements to insert into your code:
 printf("Counter is %d\n", server_counter);
 fprintf(stderr, "Server Terminated, closing connection\n");
 fprintf(stderr, "Read Error from server, closing connection\n");
 fprintf(stderr,"Error reading from stdin\n");
*/
