#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/wait.h>

int main() {
  sem_t *semaphore;
  pid_t pid;
  int status;

  semaphore = sem_open("/my_semaphore", O_CREAT, 0644, 0);
  if (semaphore == SEM_FAILED) {
    perror("Semaphore creation failed");
    return -1;
  }

  pid = fork();
  if (pid < 0) {
    perror("Fork failed");
    return -1;
  }

  if (pid == 0) {
    // Child process
    printf("Child process waiting\n");
    sem_wait(semaphore);
    printf("Child process running\n");
    sem_close(semaphore);
    return 0;
  } else {
    // Parent process
    sleep(1);
    printf("Parent process releasing semaphore\n");
    sem_post(semaphore);
    wait(&status);
    sem_close(semaphore);
    printf("Parent process done\n");
    sem_unlink("/my_semaphore");
    return 0;
  }
}

