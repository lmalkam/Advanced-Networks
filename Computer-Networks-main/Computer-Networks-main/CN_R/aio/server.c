#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include<sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
#include <signal.h>

char fifs[100][100]; //name of fifos through which client have joined
char names[100][100]; //name of the clients that joined the chat
int echod[100] = {0};
int used = 0; // total clients entered in the chat

// if any new client joins the chat
// to check and return the index from where fifo name should be taken
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

int epids[100];
//when signal occurs the hdfn function is called 
void hdfn(int sno)
{
    int i = 0;
    for(i = 0; i < used; i++)
        if(!echod[i])
            break;
    if(i == used)
        return;
    echod[i] = 1;
    epids[i] = fork();
    if(epids[i])
    {
        char inp[100];
        snprintf(inp, 100, "%s has been echoed\n", names[i]);
        for(int j = 0; j < used; j++)
        {
            if(!echod[j])
            {
                int fd = open(fifs[j], O_WRONLY);
                write(fd, inp, strlen(inp) + 1);
                close(fd);
            }
        }
    }
    else
    {
        int fd = open(fifs[i], O_WRONLY);
        dup2(fd, 1);
        char *args[] = {NULL};
        execvp("./echo", args);
    }
}

int main()
{
    signal(SIGUSR1, hdfn);
    mkfifo("p2f", 0666);
    mkfifo("ffo", 0666);
    int pp[2];
    pipe(pp);
    if(fork())
        close(pp[1]);
    else
    {
        close(pp[0]);
        dup2(pp[1], 1);
        char *args[] = {NULL};
        execvp("./p1", args);
    }
    if(!fork())
    {
        int fd = open("p2f", O_WRONLY);
        dup2(fd, 1);
        char *args[] = {NULL};
        execvp("./p2", args);
    }
    if(!fork())
    {
        char *args[] = {NULL};
        execvp("./p4", args);
    }
    FILE *fp = popen("./p3", "r");
    FILE * fp5 = popen("./p5", "w");
    printf("Server up and running\n");
    dup2(fileno(fp5), 1);
    char inp[101];
    int len = 0, fd;
    char conf[] = "Linked";
    struct pollfd fds[5];
    fds[0].fd = open("ffo", O_RDONLY | O_NONBLOCK);
    fds[0].events = POLLIN;
    fds[1].fd = pp[0];
    fds[1].events = POLLIN;
    fds[2].fd = open("p2f", O_RDONLY, O_NONBLOCK);
    fds[2].events = POLLIN;
    fds[3].fd = fileno(fp);
    fds[3].events = POLLIN;
    fds[4].fd = 0;
    fds[4].events = POLLIN;
    while(1)
    {
        inp[0] = 0;
        poll(fds, 5, -1);
        if(fds[1].revents & POLLIN)
        {
            inp[0] = 0;
            len = read(fds[1].fd, inp, 100);
            for(int i = 0; i < used; i++)
            {
                if(!echod[i])
                {
                    fd = open(fifs[i], O_WRONLY);
                    write(fd, inp, strlen(inp) + 1);
                    close(fd);
                }
            }
            write(1, inp, strlen(inp) + 1);
            fflush(stdout);
        }
        if(fds[2].revents & POLLIN)
        {
            inp[0] = 0;
            len = read(fds[2].fd, inp, 100);
            for(int i = 0; i < used; i++)
            {
                if(!echod[i])
                {
                    fd = open(fifs[i], O_WRONLY);
                    write(fd, inp, strlen(inp) + 1);
                    close(fd);
                }
            }
            write(1, inp, strlen(inp) + 1);
            fflush(stdout);
        }
        if(fds[3].revents & POLLIN)
        {
            inp[0] = 0;
            len = read(fds[3].fd, inp, 100);
            for(int i = 0; i < used; i++)
            {
                if(!echod[i])
                {
                    fd = open(fifs[i], O_WRONLY);
                    write(fd, inp, strlen(inp) + 1);
                    close(fd);
                }
            }
            write(1, inp, strlen(inp) + 1);
            fflush(stdout);
        }
        if(fds[4].revents & POLLIN)
        {
            inp[0] = 0;
            fgets(inp, 100, stdin);
            inp[strlen(inp) - 1] = '\0';
            char mes[101];
            snprintf(mes, 100, "Server: %s", inp);
            for(int i = 0; i < used; i++)
            {
                if(!echod[i])
                {
                    fd = open(fifs[i], O_WRONLY);
                    write(fd, mes, strlen(mes) + 1);
                    close(fd);
                }
            }
            write(1, inp, strlen(inp) + 1);
            fflush(stdout);
        }
        if(fds[0].revents & POLLIN)
        {
            inp[0] = 0;
            len = read(fds[0].fd, inp, 100);
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
                if(echod[ind])
                {
                    if(strcmp(op, "-1") == 0)
                    {
                        kill(epids[ind], SIGKILL);
                        char msg[100];
                        snprintf(msg, 100, "%s is back\n", names[ind]);
                        for(int i = 0; i < used; i++)
                        {
                            if(!echod[i])
                            {
                                fd = open(fifs[i], O_WRONLY);
                                write(fd, msg, strlen(msg) + 1);
                                close(fd);
                            }
                        }
                        echod[ind] = 0;
                    }
                    continue;
                }
                char mes[100];
                strcpy(mes, names[ind]);
                strcat(mes, ": ");
                strcat(mes, op);
                for(int i = 0; i < used; i++)
                {
                    if(strcmp(fifs[i], pname) && !echod[i])
                    {
                        fd = open(fifs[i], O_WRONLY);
                        write(fd, mes, strlen(mes) + 1);
                        close(fd);
                    }
                }
            }
        }
    }
    return 0;
}
