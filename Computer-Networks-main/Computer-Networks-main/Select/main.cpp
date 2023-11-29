#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>

const int MAX_BUF_SIZE = 1024;
const char *FIFO_NAME = "./myfifo";

int main() {
  int pipe_fd[2];
  if (pipe(pipe_fd) == -1) {
    std::cerr << "Error creating pipe\n";
    return 1;
  }

  if (mkfifo(FIFO_NAME, 0666) == -1) {
    std::cerr << "Error creating FIFO\n";
    return 1;
  }

  int fifo_fd = open(FIFO_NAME, O_RDONLY);
  if (fifo_fd == -1) {
    std::cerr << "Error opening FIFO\n";
    return 1;
  }

  int c = fork();
  
  if(!c){
  	dup2(pipe_fd[1], 1);
  	char* arg[] = {"./p2", NULL};
  	execv(arg[0],arg);
  }	

  fd_set read_fds;
  char buffer[MAX_BUF_SIZE];
  while (true) {
    FD_ZERO(&read_fds);
    FD_SET(pipe_fd[0], &read_fds);
    FD_SET(fifo_fd, &read_fds);
    FD_SET(STDIN_FILENO, &read_fds);

    int max_fd = std::max(pipe_fd[0], fifo_fd);
    max_fd = std::max(max_fd, STDIN_FILENO);

    int result = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);
    if (result == -1) {
      std::cerr << "Error in select()\n";
      return 1;
    }

    if (FD_ISSET(pipe_fd[0], &read_fds)) {
      int bytes_read = read(pipe_fd[0], buffer, MAX_BUF_SIZE);
      std::cout << "Data read from pipe: ";
      std::cout.write(buffer, bytes_read);
      std::cout << '\n';
    }

    if (FD_ISSET(fifo_fd, &read_fds)) {
      int bytes_read = read(fifo_fd, buffer, MAX_BUF_SIZE);
      std::cout << "Data read from FIFO: ";
      std::cout.write(buffer, bytes_read);
      std::cout << '\n';
    }

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      int bytes_read = read(STDIN_FILENO, buffer, MAX_BUF_SIZE);
      std::cout << "Data read from standard input: ";
      std::cout.write(buffer, bytes_read);
      std::cout << '\n';
    }
  }

  return 0;
}

