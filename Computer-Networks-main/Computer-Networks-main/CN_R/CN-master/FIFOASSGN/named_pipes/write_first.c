#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
//#define myfifo "./myfifo"
int main()
{
  int fd;
  char* myfifo="./readfirst";
  mkfifo(myfifo,0666);
  char arr1[80],arr2[80];
  while(1)
  {
    fd=open(myfifo,O_WRONLY);
    fgets(arr1,80,stdin);
    write(fd,arr1,strlen(arr1)+1);
    close(fd);

    fd=open(myfifo,O_RDONLY);
    read(fd,arr2,80);
    printf("user2: %s\n",arr2);
    close(fd);
  }

}
