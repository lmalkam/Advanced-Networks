#include "all_socket_functions.h"

int main(int argc,char**argv)
{
  while(!feof(stdin))
  {
    char msg[50];
    scanf("%s",msg);
    printf("%s",msg);
  }
}
