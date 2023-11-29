#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<netdb.h>
#include<sys/select.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<semaphore.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>

int a[2];

int main()
{
  a[0]=1;a[1]=2;
  int pid=fork();
  if(pid==0)
  {

    for(int i=0;i<2;i++)
      printf("%d\n",a[i] );
  }
  else
  {
    for(int i=0;i<2;i++)
      printf("hello%d\n",a[i] );
    a[0]=10;
    a[1]=20;
  }
}
