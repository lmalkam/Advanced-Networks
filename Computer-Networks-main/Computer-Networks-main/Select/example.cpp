#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>

#define BUFSIZE 1024

int main() {
    int rfd, wfd;
    int max_fd;
    char buf[BUFSIZE];
    fd_set read_fds, write_fds;
    struct timeval timeout;
    int n;

    // Open the pipe
    if ((mkfifo("myfifo", 0666) < 0) && (errno != EEXIST)) {
        perror("mkfifo");
        exit(1);
    }
    rfd = open("myfifo", O_RDONLY | O_NONBLOCK);
    wfd = open("myfifo", O_WRONLY | O_NONBLOCK);

    // Set up the file descriptor sets
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(rfd, &read_fds);
    FD_SET(wfd, &write_fds);
    max_fd = (rfd > wfd) ? rfd : wfd;

    while (1) {
        // Wait for data to be available on any of the file descriptors
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        n = select(max_fd + 1, &read_fds, &write_fds, NULL, &timeout);

        if (n < 0) {
            perror("select");
            exit(1);
        } else if (n == 0) {
            printf("Timeout\n");
        } else {
            // Check if there is data to read from the standard input
            if (FD_ISSET(STDIN_FILENO, &read_fds)) {
                fgets(buf, BUFSIZE, stdin);
                write(wfd, buf, strlen(buf));
            }
            // Check if there is data to read from the read file descriptor
            if (FD_ISSET(rfd, &read_fds)) {
                n = read(rfd, buf, BUFSIZE);
                if (n > 0) {
                    printf("Received: %.*s", n, buf);
                } else if (n == 0) {
                    printf("End of file\n");
                    break;
                } else {
                    perror("read");
                    exit(1);
                }
            }
            // Check if the write file descriptor is ready for writing
            if (FD_ISSET(wfd, &write_fds)) {
                // Do nothing for now
            }
        }
    }

    // Close the pipe and remove the FIFO file
    close(rfd);
    close(wfd);
    unlink("myfifo");

    return 0;
}
