#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include <ctype.h>
#include<iostream>
#include<bits/stdc++.h>
using namespace std;

int main()
{
  int fd[2];
  pipe(fd);
  pid_t pid=fork();
  if(pid==0)
  {
    dup2(fd[1],1);
    cout<<"rahul parihar";
    cout<<"be cool\n";
  }
  else
  {
    dup2(fd[0],0);
    char *args[]={"dup2_pipe_second",NULL};
    execv(args[0],args);
  }
}
