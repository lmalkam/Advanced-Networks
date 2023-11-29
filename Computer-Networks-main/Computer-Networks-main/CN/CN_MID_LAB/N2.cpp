#include "cn_header.h"

using namespace std;

struct msg_buf {
    long type;
    char text[BUFFER_SIZE];
} msg;

int checkport(char buf[])
{

    char temp[4] = {buf[0], buf[1], buf[2], buf[3]};
    for(int j = 0; j < 4; j++)
    {
        if (!isdigit(temp[j]))
            return -1;
    }
    return atoi(temp);
}

int connect_socket(int port_no)
{
    int sfd;
	struct sockaddr_in serv_addr;

	bzero(&serv_addr,sizeof(serv_addr));

	if((sfd = socket(AF_INET , SOCK_STREAM , 0))==-1)
	perror("\n socket");
	else printf("\n socket created successfully\n");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);
	//serv_addr.sin_addr.s_addr = INADDR_ANY;
	inet_pton(AF_INET,"127.0.0.1", &serv_addr.sin_addr);

    int nsfd = connect(sfd , (struct sockaddr *)&serv_addr , sizeof(serv_addr));

	if(nsfd==-1)
	perror("\n connect : ");
	else printf("\nconnect succesful");

    return nsfd;
}

int main(int argc, char *argv[])
{
    key_t key = ftok(".", 1234);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    sem_t *N12 = sem_open("N12", O_CREAT, 0644, 0);
    cout<<"Waiting for N1 to complete\n";
    int turn = 2;
    while(turn--)
    {

    msgrcv(msgid, &msg, sizeof(msg), 25, 0);
    sem_wait(N12);
    cout<<"Received: "<<msg.text<<"\n";
    int port = checkport(msg.text);
    port = -1;
    if( port != -1 )
    {
        int nsfd = connect_socket(port);
        write(nsfd, msg.text,strlen(msg.text));
    }
    else{
        mkfifo("NtoS",0666);
        int fd = open("./NtoS", O_WRONLY);

        write(fd, msg.text, strlen(msg.text));
    }
    printf("Data Received is : %s \n", msg.text);
    msgctl(msgid, IPC_RMID, NULL);
    }
  
    return 0;
}
