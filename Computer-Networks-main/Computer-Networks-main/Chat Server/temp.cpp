#include <sys/select.h>
#include <unistd.h>
#include <iostream>

int main() {
  fd_set read_fds;
  FD_ZERO(&read_fds);
  FD_SET(STDOUT_FILENO, &read_fds);

  int max_fd = STDOUT_FILENO;
  struct timeval timeout;
  timeout.tv_sec = 10;
  timeout.tv_usec = 0;

  int result = select(max_fd + 1, &read_fds, nullptr, nullptr, &timeout);
  if (result == -1) {
    std::cerr << "Error in select()\n";
    return 1;
  }

  if (FD_ISSET(STDOUT_FILENO, &read_fds)) {
    std::cout << "Standard output is ready for writing\n";
  } else {
    std::cout << "Standard input is not ready\n";
  }

  return 0;
}

