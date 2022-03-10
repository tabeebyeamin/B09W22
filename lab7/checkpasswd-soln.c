#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main() {
    char userid[10];
    char password[10];
    printf("User id:\n");
    scanf("%s", userid);
    printf("Password:\n");
    scanf("%s", password);

    // Create a file descriptor to use for the pipe
    int fd[2];
    int pid;
    
    /* Pipe and fork error checks
    */
    if(pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }
    if((pid = fork()) == -1) {
        perror("fork");
        exit(1);
    } 
    /*
    child executes validate, closes the end of the pipe
    it doesn't need. (good practice)
    */
    else if(pid == 0) {
        // receiving data from parent process
        close(fd[1]);

        // stdin into the validate function
        dup2(fd[0], fileno(stdin));

        // execlp means you need exact path 
        execlp("./validate", "./validate", NULL);

        // note, the code after exec doesn't get called.
        perror("exec");
        exit(1);

    /*
    Parent writes to the other end of the pipe, closing
    the child's end

    It waits for the child to finish, closes its own end
    after it's done
    */
    } else {
        
        close(fd[0]);

        // writing the data to its end, twice 
        // (read validate to see why)
        write(fd[1], userid, 10);
        write(fd[1], password, 10);
        
        close(fd[1]);
        int status;
        
        // parent waits for child to terminate exec
        // wait error check
        if((wait(&status)) == -1) {
            perror("wait");
            exit(1);


        } else {
            // if child exited
            if(WIFEXITED(status)) {
                /*
                Successful match if child exited with a status of 0
                (check validate.c to see how this works)
                */
                if(WEXITSTATUS(status) == 0){
                    printf("Password verified\n");
                } else {
                    printf("Either password or user id is incorrect\n");
                }
            }
        }
    }
    return 0;
}