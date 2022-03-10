#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 256

#define PASSWORD_FILE "pass"

/* Reads two 10 character chunks from stdin, and checks if they match a user id
and password pair from a password file. The first 10 bytes read will contain a
user id, and the second 10 bytes will contain a password. The program exits with
a value of 0 if the user id and password match, 1 if there is an error, 2 if the
user id is not found or the password does not match. */

int main(){
    int n;
    char userid[30];
    char password[10];
    
    if((n = read(fileno(stdin), userid, 10)) == -1) {
        perror("read");
        exit(1);
    } else if(n == 0) {;
        fprintf(stderr, "Error: could not read from stdin");
        exit(1);
    } 
    if((n = read(fileno(stdin), password, 10)) == -1) {
        perror("read");
        exit(1);
    } else if(n == 0) {;
        fprintf(stderr, "Error: could not read from stdin");
        exit(1);
    } 
    
    strncat(userid, ":", sizeof(userid));
    strncat(userid, password, sizeof(userid));
    FILE *fp = fopen(PASSWORD_FILE, "r");
    char line[MAXLINE];
    while(fgets(line, sizeof(line), fp)) {
        line[strlen(line) - 1] = '\0';
        if(strcmp(userid, line) == 0){
            exit(0); // found match
        }
    }
    exit(2);
}
