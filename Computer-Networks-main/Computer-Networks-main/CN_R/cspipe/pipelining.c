#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
int main()
{
    int p[2];
    pipe(p);

    char n[100];
    scanf("%s",n);
    dup2(p[0],0);
    write(p[1],n,strlen(n));
    close(p[1]);
    FILE *fd1=popen("./a","r");
    dup2(fileno(fd1),0);
    fd1=popen("./b","r");
    dup2(fileno(fd1),0);
    fd1=popen("./c","r");
    dup2(fileno(fd1),0);
    fd1=popen("./d","r");
    char c;
    while((c = fgetc(fd1)) != EOF) {
      putchar(c);
    }
}
