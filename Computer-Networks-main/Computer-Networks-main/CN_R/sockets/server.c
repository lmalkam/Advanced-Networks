#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int sfd;
struct sockaddr_in ad;
int nsfd[100];
struct sockaddr_in ninfo[100];
int nlen[100];
int used = 0;

struct msg_st
{
    char name[100];
    char data[100];
} msg;

int main()
{
    int opt = 1;
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = INADDR_ANY;
    ad.sin_port = htons(PORT);
    if (bind(sfd, (struct sockaddr *)&ad, sizeof(ad)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(sfd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server up and running\n");
    char conf[] = "Linked";
    struct pollfd fds[101];
    fds[0].fd = sfd;
    fds[0].events = POLLIN;
    while (1)
    {
        int ret = poll(fds, used + 1, -1);
        if (!ret)
            continue;
        for (int i = 0; i < used; i++)
        {
            if (fds[i + 1].revents & POLLIN)
            {
                read(nsfd[i], &msg, sizeof(msg));
                for (int j = 0; j < used; j++)
                    if (j != i)
                        write(nsfd[j], &msg, sizeof(msg));
            }
        }
        if (fds[0].revents & POLLIN)
        {
            if ((nsfd[used] = accept(sfd, (struct sockaddr *)&ninfo[used], (socklen_t *)&nlen[used])) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            strcpy(msg.data, conf);
            write(nsfd[used], &msg, sizeof(msg));
            used++;
            fds[used].fd = nsfd[used - 1];
            fds[used].events = POLLIN;
        }
    }
    return 0;
}
