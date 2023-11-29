#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
  int n = argc - 1;
  int fd[2];
  pid_t pid;

  for (int i = 0; i < n - 1; i++) {
    pipe(fd);
    pid = fork();
    if (pid == 0) {
      // child process
      dup2(fd[0], STDIN_FILENO);
      close(fd[0]);
      close(fd[1]);
      if (execvp(argv[i + 2], &argv[i + 2]) < 0) {
        perror("execvp");
        exit(1);
      }
    } else if (pid < 0) {
      // error
      perror("fork");
      exit(1);
    } else {
      // parent process
      dup2(fd[1], STDOUT_FILENO);
      close(fd[0]);
      close(fd[1]);
      wait(NULL);
    }
  }
  if (execvp(argv[1], &argv[1]) < 0) {
    perror("execvp");
    exit(1);
  }
  return 0;
}

