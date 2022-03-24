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

    // Timer
    struct itimerval timer;
    // set the seconds to expire after
    timer.it_value.tv_sec = seconds;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    // you can use either ITIMER_REAL/VIRTUAL/PROF
    if (setitimer(ITIMER_PROF, &timer, NULL) != 0) {
        perror("timer");
        exit(1);
    }
    // changing signal
    struct sigaction newact;
    // set the handling function
    newact.sa_handler = handler;
    newact.sa_flags = 0;

    sigemptyset(&newact.sa_mask);

    /*
    The signal generated is either SIGALRM, SIGVTALRM or SIGPROF
    depending on which timer you use.
    */
    sigaction(SIGPROF, &newact, NULL);
    /* In an infinite loop, read an int from a random location in the file,
     * and print it to stderr.
     */
    while (1) {
        int num, random_num;
        random_num = random()%100;
        // you can find more about how fseek() is used in the link in helpful.txt
        fseek(fp, random_num*sizeof(int), SEEK_SET);
        
        // read to &num, and print if successful
        if (fread(&num, sizeof(int), 1, fp) < 0) {
            perror("read");
            exit(1);
        }
        fprintf(stderr, "%d\n", num);
        num_reads += 1;
    }
    return 1; // something is wrong if we ever get here!
}