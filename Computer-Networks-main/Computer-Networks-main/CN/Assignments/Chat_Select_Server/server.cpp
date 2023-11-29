#include "cn_header.h"

#define MAX_CLIENT 3
using namespace std;

int sfd[MAX_CLIENT];
int nsfd[MAX_CLIENT];
int PORT[MAX_CLIENT] = {3000, 3001, 3002};

pthread_t thd1, thd2, thd3;
fd_set r_fds, w_fds, temp_rfds, temp_wfds;

void setup_server(int cur)
{
    if( cur >= MAX_CLIENT )
        return;
    struct sockaddr_in address;
    int opt = 1;
    if ((sfd[cur] = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(sfd[cur], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT[cur]);

    if (bind(sfd[cur], (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(sfd[cur], 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

void response(int cur)
{
    char buf[1024] = {0};
    read(nsfd[cur], buf, sizeof(buf));
    cout<<"Message Received from Client "<<cur<<": "<<buf<<endl;
    
    if( cur )
    {
        if(cur == 1)
        {
            for(int i=0; i<strlen(buf); i++)
            {
                buf[i] = tolower(buf[i]);
            }
        }
        else{
            for(int i=0; i<strlen(buf); i++)
            {
                buf[i] = toupper(buf[i]);
            }
        }

        send(nsfd[cur], buf, sizeof(buf), 0);
    }
}

void* fun1(){
    while(1)
    {
        cout<<"Message Sent to Client 1\n";
    }
}

void* fun2(){
    while(1)
    {
        cout<<"Message Sent to Client 2\n";        
    }
}

void* fun3(){
    while(1)
    {
        cout<<"Message Sent to Client 3\n";        
    }
}

int main()
{
    char buff[1024];
    socklen_t len[3] = {sizeof(addr[0])};
    for (int cur = 0 ; cur < MAX_CLIENT ; cur++)
    {
        setup_server(cur);
    }

    FD_ZERO(&r_fds);
    FD_ZERO(&w_fds);
    for(int cur = 0 ; cur < MAX_CLIENT ; cur++)
    {
        if ((nsfd[cur] = accept(sfd[cur], (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        FD_SET(nsfd[cur], &r_fds);

        // hadndling connection less fds
        if( cur != MAX_CLIENT - 1 ) 
        {
            FD_SET(nsfd[cur], &w_fds);
        }
    }

    temp_wfds = w_fds;
    temp_rfds = r_fds;    
 
    while(1)
    {
        w_fds = temp_wfds;
        r_fds = temp_rfds;

        int cnt = select(FD_SETSIZE, &rfds, &wfds, NULL, NULL);
        
        if(cnt > 0)
        {
            for(int cur = 0 ; cur < MAX_CLIENT ; cur++ )
            {
                if( FD_ISSET(nsfd[cur], &r_fds) )
                {
                    response(cur);
                }
            }
        }
        else break;
    }
    pthread_create(&thd1, NULL, fun1, NULL);
    pthread_create(&thd2, NULL, fun2, NULL);
    pthread_create(&thd3, NULL, fun3, NULL);

    pthread_join(thd1, NULL);
    pthread_join(thd2, NULL);
    pthread_join(thd3, NULL);
}