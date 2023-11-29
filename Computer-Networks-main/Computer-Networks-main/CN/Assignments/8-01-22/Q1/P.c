#include <stdio.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

void check_kill(char *ch, int id)
{
   if(strcmp(ch, "\n") == 0 || strcmp(ch, "\0") == 0) {
      printf("\n**NULL CHARACTER ENTERED**\n\n");
      kill(id, SIGABRT);
   }
}

int main() 
{
   int pwcr_pipe[2];    // parent write, child read pipe
   int prcw_pipe[2];    // parent read, child write pipe
   char *messsage_1, *messsage_2;
   size_t size = BUFFER_SIZE;

   // allocating message size
   messsage_1 = (char *) malloc (size);
   messsage_2 = (char *) malloc (size);

   pipe(pwcr_pipe);
   pipe(prcw_pipe);
   
   int pd = fork();
   
   if (pd > 0) 
   {
      close(pwcr_pipe[0]);
      close(prcw_pipe[1]);
      int pid = getpid();
      while(1)
      {

         // sending message to child
         printf("P) \"Send\" Message from Parent [%d]: ",pid);
         getline(&messsage_1, &size, stdin);
         write(pwcr_pipe[1], messsage_1, BUFFER_SIZE);

         // receiving message from  child
         read(prcw_pipe[0], messsage_2, BUFFER_SIZE);
         check_kill(messsage_2, pid);
         printf("P) Message \"Received\" from Child: %s\n",messsage_2);
      }

   } 
   else{

      close(pwcr_pipe[1]); 
      close(prcw_pipe[0]); 
      int pid = getpid();
      int ppid = getppid();
      while(1)
      {
         // receiving message from parent
         read(pwcr_pipe[0], messsage_1, BUFFER_SIZE);
         check_kill(messsage_1, ppid);
         printf("P') Message \"Received\" from Parent [%d]: %s\n", ppid, messsage_1);

         // sending message to parent
         printf("P') \"Send\" Message from Child [%d]: ",pid);
         getline(&messsage_2, &size, stdin);
         write(prcw_pipe[1], messsage_2, BUFFER_SIZE);
      }
   }
   return 0;
}