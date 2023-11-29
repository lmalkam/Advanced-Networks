#include "cn_header.h"
#define BUFFER_SIZE 256
#define MKFIFO_PERM 0666

void check_kill(char ch[], int id)
{
   if(strcmp(ch, "\n") == 0 || strcmp(ch, "\0") == 0) {
      printf("\n**NULL CHARACTER ENTERED**\n\n");
      kill(id, SIGABRT);
   }
}

void* write_channel()
{
    char message_1[BUFFER_SIZE];
    size_t size = BUFFER_SIZE;

    messsage_1 = (char *) malloc (size);

    printf("P1.) Enter Message : \n");
    getline(&messsage_1, &size, stdin);
    
    int fd=popen("./P2","W");
    check_kill(message_1, fd);

    write(fd, message_1, BUFFER_SIZE);
    close(fd);
}

void* read_channel()
{
    char *message_2;
    size_t size = BUFFER_SIZE;

    messsage_2 = (char *) malloc (size);
    int ffd=open("ffifo",O_RDONLY);
    read(ffd,message_2,BUFFER_SIZE);
    printf("P1.) Message sent : %s \n",message_2);
    close(ffd);
}

int main()
{
    int ffd= mkfifo("fifo", MKFIFO_PERM);
    pthread_t write_thread, read_thread;
    pthread_create(&write_thread,NULL,write_channel,NULL);
    pthread_create(&read_thread,NULL,read_channel,NULL);
    pthread_join(s,NULL);
    pthread_join(r,NULL);
}

