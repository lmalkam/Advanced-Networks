#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

char fifs[100][100];
char names[100][100];
int used = 0;

int newcon(char *s)
{
    int l = strlen(s);
    if(l <= 4)
        return 0;
    if(s[l - 1] != 'k' || s[l - 2] != 'n' || s[l - 3] != 'i' || s[l - 4] != 'l')
        return 0;
    int space = l - 5;
    for(; space >= 0 && s[space] != ' '; space--)
        if(s[space] < '0' || s[space] > '9')
            return 0;
    return space;
}

int main()
{
    mkfifo("ffo", 0666);
    char inp[101];
    int len = 0;
    printf("Server up and running\n");
    int fd;
    char conf[] = "Linked";
    while(1)
    {
        fd = open("ffo", O_RDONLY);
        len = read(fd, inp, 100);
        close(fd);
        int sp;
        if(sp = newcon(inp))
        {
            char name[100], ln[100];
            int nu = 0, lu = 0;
            for(int i = sp + 1; i < strlen(inp); i++)
                ln[lu++] = inp[i];
            ln[lu] = '\0';
            for(int i = 0; i < sp; i++)
                name[nu++] = inp[i];
            name[nu] = '\0';
            strcpy(fifs[used], ln);
            strcpy(names[used], name);
            fd = open(ln, O_WRONLY);
            write(fd, conf, strlen(conf) + 1);
            close(fd);
            used++;
            strcat(name, " has entered the chat\n");
            for(int i = 0; i < used - 1; i++)
            {
                fd = open(fifs[i], O_WRONLY);
                write(fd, name, strlen(name) + 1);
                close(fd);
            }
        }
        else
        {
            char pname[100], op[100];
            int l = 0;
            while(l < strlen(inp) && inp[l] != 'k')
            {
                pname[l] = inp[l];
                l++;
            }
            if(inp[l] == 'k')
            {
                pname[l] = 'k';
                l++;
            }
            else continue;
            if(l == strlen(inp))
                continue;
            pname[l] = '\0';
            for(int i = l; i < strlen(inp); i++)
                op[i - l] = inp[i];
            op[strlen(inp) - l] = '\0';
            int c = 0;
            int ind = -1;
            for(int i = 0; i < used; i++)
            {
                if(strcmp(fifs[i], pname) == 0)
                {
                    ind = i;
                    break;
                }
            }
            if(ind == -1)
                continue;
            char mes[100];
            strcpy(mes, names[ind]);
            strcat(mes, ": ");
            strcat(mes, op);
            for(int i = 0; i < used; i++)
            {
                if(strcmp(fifs[i], pname))
                {
                    fd = open(fifs[i], O_WRONLY);
                    write(fd, mes, strlen(mes) + 1);
                    close(fd);
                }
            }
        }
    }
    return 0;
}
