#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = getpid();
    pid_t pgid = getpgid(pid);
    printf("Parent process: PID=%d, PGID=%d\n", pid, pgid);

    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        // Child process
        pid_t child_pid = getpid();
        // Set the child process's PGID to its own PID
        /*if (setpgid(child_pid, child_pid) == -1) {
            perror("setpgid");
            exit(EXIT_FAILURE);
        }*/
        pid_t child_pgid = getpgid(child_pid);
        printf("Child process: PID=%d, PGID=%d\n", child_pid, child_pgid);
        // Do some work here...
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        pid_t child_pgid = child_pid;
        // Wait for child process to exit
        int status;
        waitpid(child_pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        }
        printf("Parent process: PID=%d, PGID=%d\n", pid, pgid);
        // Do some work here...
        exit(EXIT_SUCCESS);
    }
}

