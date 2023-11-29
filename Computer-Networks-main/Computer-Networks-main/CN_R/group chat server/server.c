#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include<poll.h>

char fifs[100][100][100];
char names[100][100][100];
char gnames[100][100];

int used[100] = {0};
char cnames[100][100];
int gused=0;

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
    struct pollfd fds[101];
    fds[0].fd=open("ffo",O_RDONLY|O_NONBLOCK);
    fds[0].events=POLLIN;
    while(1)
    {
        int ret=poll(fds,gused+1,-1);
        if(!ret)
          continue;
        if(fds[0].revents&POLLIN)
        {
          char inp[101];
          len = read(fds[0].fd,inp,sizeof(inp));
          int sp;
          if(sp = newcon(inp))
          {
            char name[100], ln[100],gname[100];
            int nu = 0, lu = 0,gnu=0;
            for(int i = sp + 1; i < strlen(inp); i++)
                ln[lu++] = inp[i];
            ln[lu] = '\0';
            int i;
            for(i=0;inp[i]!='/';i++)
              name[nu++]=inp[i];
            name[nu]='\0';
            i++;
            for(; i < sp; i++)
                gname[gnu++] = inp[i];
            gname[gnu] = '\0';
            int ng=1;
            for(i=0;i<gused;i++)
            {
              if(strcmp(gname,gnames[i])==0)
              {
                ng=0;
                strcpy(fifs[i][used[i]],ln);
                strcpy(names[i][used[i]],name);
                char im[100];
                strcpy(im,conf);
                strcat(im,cnames[i]);
                fd=open(ln,O_WRONLY);
                write(fd,im,sizeof(im));
                close(fd);
                used[i]++;
                strcat(name," has entered the chat\n");
                for(int j=0;j<used[i]-1;j++)
                {
                  fd=open(fifs[i][j],O_WRONLY);
                  write(fd,name,sizeof(name));
                  close(fd);
                }
                break;
              }
            }
            if(ng)
            {
              strcpy(fifs[gused][0], ln);
              strcpy(names[gused][0], name);
              int lnn = strlen(ln);
              ln[lnn - 4] = 'c';
              ln[lnn - 3] = 'h';
              ln[lnn - 2] = 'a';
              ln[lnn - 1] = 't';
              strcpy(cnames[gused], ln);
              strcpy(gnames[gused], gname);
              printf("New group %s created\n", gname);
              mkfifo(cnames[gused], 0666);
              char im[100];
              strcpy(im, conf);
              strcat(im, cnames[gused]);
              fd = open(fifs[gused][0], O_WRONLY);
              write(fd, im, strlen(im) + 1);
              close(fd);
              used[gused] = 1;
              gused++;
              fds[gused].fd = open(cnames[gused - 1], O_RDONLY | O_NONBLOCK);
              fds[gused].events = POLLIN;
              fds[gused].revents = 0;
            }
          }
        }
        for(int it=1;it<=gused;it++)
        {
          if(fds[it].revents & POLLIN)
          {
              char inp[101];
              len = read(fds[it].fd, inp, 100);
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
              for(int i = 0; i < used[it - 1]; i++)
              {
                  if(strcmp(fifs[it - 1][i], pname) == 0)
                  {
                      ind = i;
                      break;
                  }
              }
              if(ind == -1)
                  continue;
              char mes[100];
              strcpy(mes, names[it - 1][ind]);
              strcat(mes, ": ");
              strcat(mes, op);
              for(int i = 0; i < used[it - 1]; i++)
              {
                  if(strcmp(fifs[it - 1][i], pname))
                  {
                      fd = open(fifs[it - 1][i], O_WRONLY);
                      write(fd, mes, strlen(mes) + 1);
                      close(fd);
                  }
              }
            }
        }
    }
    return 0;
}
