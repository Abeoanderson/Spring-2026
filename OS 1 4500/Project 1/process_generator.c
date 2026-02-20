#include <stdio.h>
#include <unistd.h>     // for fork(), getpid(), sleep()
#include <sys/types.h>
#include <sys/wait.h>   // for wait()

int main() {
    pid_t pid;

    printf("Parent process started with PID: %d\n", getpid());

    pid = fork();
    
    if (pid < 0) {
        // Fork failed
        perror("fork failed");
        return 1;
    }

    else if (pid == 0) {
        // child process is successful
        printf("Child running with PID: %d\n", getpid());
        printf("Child's parent PID: %d\n", getppid());
        // sleep child for 2 min so we can inspect it
        sleep(120);
        printf("Child exiting.\n");
        // all done with child
    } 
    else {
        // rest of parent logic
        printf("Parent Waiting... \n");
        // wait on child
        wait(NULL);
        // all done
        printf("Parent finished waiting. Child done.\n");

    }
    return 0;

}