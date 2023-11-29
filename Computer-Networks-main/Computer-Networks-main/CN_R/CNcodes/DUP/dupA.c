#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

int main()
{
  char rfname[]="read.txt";
  char wfname[]="write.txt";
  FILE *rf=fopen(rfname,"r");
  FILE *wf=fopen(wfname,"w");
  int rfd=fileno(rf);
  int wfd=fileno(wf);
  dup2(rfd,0);
  dup2(wfd,1);
  FILE *F=popen("./p2","r");
  if(F==NULL) printf("xx");
  int fd1=fileno(F);
  char buf[100];
  while(read(fd1,buf,100)>0)
  {
    printf("%s",buf);
  }
  fflush(stdout);
}
