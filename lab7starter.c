#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    // Declare any new variables you need



    // Write the code to loop over the command line arguments.
    for (int i = 0; i < argc-1; i++) {
        // Before we call fork, call pipe

        

        int result = fork();
        if (result < 0) {
            perror("fork");
            exit(1);
        } else if (result == 0) { // child process
            // Child only writes to the pipe, so close reading end




            // Before we forked, parent had open the reading ends to
            // all previously forked children; so close those.




            // Now do the work - write the value in binary to the pipe
            int len = strlen(argv[i+1]);



            // Close the pipe since we are done with it.



            exit(0); // Don’t fork children on next loop iteration
        } else {
            // In the parent, but before doing the next loop iteration,
            // close the end of the pipe that we don’t want open




        }
    }
    // Only the parent gets here
    int sum = 0;
    // Read one integer from each child, print it and add to sum



    printf("The length of all the args is %d\n", sum);
    return 0;
}
