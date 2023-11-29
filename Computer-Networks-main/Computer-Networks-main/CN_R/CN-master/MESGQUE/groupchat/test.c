#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/unistd.h>
int main()
{
  char str[10];
  long int a=123;
  sprintf(str, "%ld", a);
  printf("%s\n",str );
}
