#include "cn_header.h"
#define BUFFER_SIZE 256

void check_kill(char ch[], int id)
{
   if(strcmp(ch, "\n") == 0 || strcmp(ch, "\0") == 0) {
      printf("\n**NULL CHARACTER ENTERED**\n\n");
      kill(id, SIGABRT);
   }
}

int main(){
    char *messsage;
    size_t size = BUFFER_SIZE;
    messsage = (char *) malloc (size);

    printf("Enters P2 \n"); 

    while(1)
    {
      int pfd = popen("./P3","W");
      dup2(pfd,1);
      getline(&messsage, &size, stdin);
      check_kill(message, pfd);
      close(pfd);
      printf("%s\n",message);
    }
}