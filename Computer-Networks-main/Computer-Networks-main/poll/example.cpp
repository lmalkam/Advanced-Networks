#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>

using namespace std;

int main() {
    const char* FIFO_NAME = "mypipe";
    const int NUM_FDS = 2;
    int fd[NUM_FDS];
    struct pollfd fds[NUM_FDS];

    // Create named pipe
    mkfifo(FIFO_NAME, 0666);

    // Open read-end of named pipe
    fd[0] = open(FIFO_NAME, O_RDONLY | O_NONBLOCK);
    if (fd[0] == -1) {
        cerr << "Error opening read-end of named pipe." << endl;
        exit(EXIT_FAILURE);
    }

    // Open write-end of named pipe
    fd[1] = open(FIFO_NAME, O_WRONLY | O_NONBLOCK);
    if (fd[1] == -1) {
        cerr << "Error opening write-end of named pipe." << endl;
        exit(EXIT_FAILURE);
    }

    // Initialize pollfd structures
    memset(fds, 0, sizeof(fds));
    fds[0].fd = fd[0];
    fds[0].events = POLLIN;
    fds[1].fd = fd[1];
    fds[1].events = POLLOUT;

    // Wait for events on the file descriptors
    while (true) {
        int num_events = poll(fds, NUM_FDS, -1);
        if (num_events == -1) {
            cerr << "Error in poll." << endl;
            exit(EXIT_FAILURE);
        }
        else if (num_events == 0) {
            cout << "No events received." << endl;
        }
        else {
            // Check for read event on read-end of named pipe
            if (fds[0].revents & POLLIN) {
                char buf[100];
                int num_bytes = read(fd[0], buf, sizeof(buf));
                if (num_bytes == -1) {
                    cerr << "Error reading from named pipe." << endl;
                }
                else {
                    buf[num_bytes] = '\0';
                    cout << "Received message: " << buf << endl;
                }
            }

            // Check for write event on write-end of named pipe
            if (fds[1].revents & POLLOUT) {
                const char* msg = "Hello, world!";
                int num_bytes = write(fd[1], msg, strlen(msg));
                if (num_bytes == -1) {
                    cerr << "Error writing to named pipe." << endl;
                }
                else {
                    cout << "Sent message: " << msg << endl;
                }
            }
        }
    }

    // Close file descriptors and delete named pipe
    close(fd[0]);
    close(fd[1]);
    unlink(FIFO_NAME);

    return 0;
}
