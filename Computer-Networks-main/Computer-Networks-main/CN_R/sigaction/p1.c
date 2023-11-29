#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>

void handler(int sig,siginfo_t *siginfo,void *context)
{
  fflush(stdout);
  printf("Sending PID: %ld,UID: %ld\n",(long)siginfo->si_pid,(long)siginfo->si_uid);

}
int main()
{
  struct sigaction act;
  memset(&act,'\0',sizeof(act));
  act.sa_sigaction = &handler;
  act.sa_flags=SA_SIGINFO;
  if(sigaction(SIGUSR1,&act,NULL)<0)
  {
    perror("sigaction\n");
    exit(1);
  }
  while(1)
  {
    sleep(1);
    printf("working\n");
    fflush(stdout);
  }
  return 0;

}
