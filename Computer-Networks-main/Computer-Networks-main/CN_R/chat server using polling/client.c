#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include<poll.h>
pthread_t rdth, wrth;
int wrfd, rdfd, pi;
char s[100];

void *rf(void *args)
{
    char inp[101];
    while(1)
    {
        rdfd = open(s, O_RDONLY);
        read(rdfd, inp, 100);
        close(rdfd);
        printf("%s\n", inp);
    }
}

void *wf(void *args)
{
    char inp[100], fin[101];
    while(1)
    {
        inp[0] = '\0';
        fgets(inp, 100, stdin);
        inp[strlen(inp) - 1] = '\0';
        strcpy(fin, s);
        strcat(fin, inp);
        wrfd = open("ffo", O_WRONLY);
        write(wrfd, fin, strlen(fin) + 1);
        close(wrfd);
    }
}

int main()
{
    char name[100];
    printf("Enter your name\n");
    fgets(name, 100, stdin);
    name[strlen(name) - 1] = '\0';
    pi = getpid();
    snprintf(s, 10, "%d", pi);
    int l = strlen(s);
    s[l++] = 'l';
    s[l++] = 'i';
    s[l++] = 'n';
    s[l++] = 'k';
    s[l] = '\0';
    wrfd = open("ffo", O_WRONLY);
    mkfifo(s, 0666);
    char im[100];
    strcpy(im, name);
    strcat(im, " ");
    strcat(im, s);
    write(wrfd, im, strlen(im) + 1);
    close(wrfd);

    //polling
    struct pollfd fds[2];
    fds[0].events=POLLIN;
    fds[1].events=POLLIN;

    fds[0].fd=open(s,O_RDONLY);
    fds[1].fd=0; //standard input 0
    while(1)
    {
      int ret=poll(fds,2,500);
      if(ret==-1)
        continue;
      if(fds[0].revents&POLLIN)
      {
        char inp[100];
        read(fds[0].fd,inp,sizeof(inp));
        printf("%s\n",inp);
      }
      if(fds[1].revents&POLLIN)
      {
        char inp[100],fin[101];
        inp[0] = '\0';
        fgets(inp, 100, stdin);
        inp[strlen(inp) - 1] = '\0';
        strcpy(fin, s);
        strcat(fin, inp);
        wrfd = open("ffo", O_WRONLY);
        write(wrfd, fin, strlen(fin) + 1);
        close(wrfd);
      }
    }
    return 0;
}
