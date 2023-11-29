#include "cn_header.h"

using namespace std;

struct sockaddr_in serv_addr;

struct msg_buf {
    long type;
    char text[BUFFER_SIZE];
} msg;

int checkport(char buf[])
{

    char temp[5] = {buf[0], buf[1], buf[2], buf[3]};
    if(temp[0] > 64)
        return -1;
    for(int j = 0; j < 4; j++)
    {
        cout<<temp[j]<<" ";
        if (!(temp[j] >= '0' && temp[j] <= '9'))
            return -1;
    }
    cout<<"GONE";
    return atoi(temp);
}

int connect_socket(int port_no)
{
    int sfd;
	// struct sockaddr_in serv_addr;

	bzero(&serv_addr,sizeof(serv_addr));

	if((sfd = socket(AF_INET , SOCK_STREAM , 0))==-1)
	perror("\n socket");
	else printf("\n \nsocket created successfully\n\n");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);
	//serv_addr.sin_addr.s_addr = INADDR_ANY;
	inet_pton(AF_INET,"127.0.0.1", &serv_addr.sin_addr);

    int nsfd = connect(sfd , (struct sockaddr *)&serv_addr , sizeof(serv_addr));

	if(nsfd==-1)
	perror("\n connect : ");
	else printf("\nconnect succesful\n");

    return nsfd;
}

int main(int argc, char *argv[])
{
    key_t key = ftok(".", 1234);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    sem_t *N1S = sem_open("N12", O_CREAT, 0644, 0);
    int turn = 1;
    while(turn--)
    {
        msgrcv(msgid, &msg, sizeof(msg), 20, 0);

    printf("Data Received is : %s \n\n", msg.text);
    // int port = checkport(msg.text);
    int port = 8080;
    if( port != -1 )
    {
        char ch[5] = "sufi";
        int nsfd = connect_socket(port);
        sendto(nsfd , ch,5 , 0 , ( struct sockaddr * ) &serv_addr ,  sizeof(serv_addr));
        // write(nsfd, ch, sizeof(ch));
        sem_post(N1S);
    }
    else{
        cout<<"Help";
        mkfifo("NtoS",0666);
        int fd = open("./NtoS", O_WRONLY);

        write(fd, msg.text, strlen(msg.text));
    }
    sem_post(N1S);
    }
    msgctl(msgid, IPC_RMID, NULL);
  
    return 0;
}
