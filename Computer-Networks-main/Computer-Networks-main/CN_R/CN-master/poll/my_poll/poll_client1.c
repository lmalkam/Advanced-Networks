#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>

int main()
{
  while (1) {
    printf("this is coming from the first client\n");
    sleep(2);
  }
  return 0;
}
