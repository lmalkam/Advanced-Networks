#include "cn_header.h"
#define BUFFER_SIZE 256

void check_kill(char ch[], int id)
{
   if(strcmp(ch, "\n") == 0 || strcmp(ch, "\0") == 0) {
      printf("\n**NULL CHARACTER ENTERED**\n\n");
      kill(id, SIGABRT);
   }
}

int main()
{
    char *messsage_1;
    size_t size = BUFFER_SIZE;
    messsage_1 = (char *) malloc (size);

    while(1)
    {
        int ffd=open("ffifo",O_WRONLY);
        getline(&messsage_1, &size, stdin);
        check_kill(messsage_1, ffd);

        char *messsage_2;
        messsage_2 = (char *) malloc (size);

        for(int cur=0; cur < messsage_1[cur]!='\0'; cur++)
            messsage_2[cur] = toupper(messsage_1[cur]);

        printf("%s\n",messsage_2);
        char temp[] = "From P3";
        strcat(messsage_1, temp);
        
        write(ffd,messsage_1,BUFFER_SIZE);
        close(ffd);
    }
}