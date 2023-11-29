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

  FILE *file =popen("./popenp2","r");

  char* msg="sushil bsdk ka ";
  int fd= fileno(file);
  string s;
  dup2(fd,0);
  cin>>s;
  cout<<s;
  cout<<"\nduniya";
  close(fd);
  pclose(file);

}
