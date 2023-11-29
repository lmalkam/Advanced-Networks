#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    while(1)
    {
        char mes[101];
        read(0, mes, 100);
        printf("In p5: %s\n", mes);
        fflush(stdout);
    }
    return 0;
}
