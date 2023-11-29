#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
  while(1)
  {
    char buff[100];
    read(0,buff,100);
    int i=0;
    //tolower()
    while(buff[i]!='\0')
    {
      if(buff[i]>='A'&&buff[i]<='Z')
        buff[i]=buff[i]+32;
      i++;
    }
    write(1,buff,strlen(buff)+1);
  }
  return 0;
}
