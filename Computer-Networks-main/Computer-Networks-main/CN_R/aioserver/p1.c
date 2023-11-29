#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    while(1)
    {
        char mes[] = "Greetings from p1\n";
        write(1, mes, strlen(mes) + 1);
        fflush(stdout);
        sleep(10 + rand() % 10);
    }
    return 0;
}
