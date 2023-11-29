#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    sleep(10);
    while(1)
    {
        kill(getppid(), SIGUSR1);
        sleep(20);
    }
    return 0;
}
