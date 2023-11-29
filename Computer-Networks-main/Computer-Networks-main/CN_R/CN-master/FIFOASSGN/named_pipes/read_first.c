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
  int fd2;
  char* myfifo="./readfirst";
  mkfifo(myfifo,0666);
  char str1[80],str2[80];
  while(1)
  {
    fd2=open(myfifo,O_RDONLY);
    read(fd2,str1,80);
    printf("user1: %s\n",str1);
    close(fd2);

    fd2=open(myfifo,O_WRONLY);
    fgets(str2,80,stdin);
    write(fd2,str2,strlen(str2)+1);
    close(fd2);
  }

}
