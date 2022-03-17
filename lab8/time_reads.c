/* The purpose of this program is to practice writing signal handling
 * functions and observing the behaviour of signals.
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

/* Message to print in the signal handling function. */
#define MESSAGE "%ld reads were done in %ld seconds.\n"

/* Global variables to store number of read operations and seconds elapsed. 
 */
long num_reads, seconds;

// Signali handler
void handler(int code) {
    fprintf(stdout, MESSAGE, num_reads, seconds);
    exit(0);
}

/* The first command line argument is the number of seconds to set a timer to run.
 * The second argument is the name of a binary file containing 100 ints.
 * Assume both of these arguments are correct.
 */

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: time_reads <seconds> <filename>\n");
        exit(1);
    }

    seconds = strtol(argv[1], NULL, 10);
    FILE *fp;
    if ((fp = fopen(argv[2], "r")) == NULL) {
      perror("fopen");
      exit(1);
    }


    while (1) {
        printf("I'm just looping until you stop me.\n");
    }
    return 1; // something is wrong if we ever get here!
}