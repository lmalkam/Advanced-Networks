#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include<signal.h>

struct message_buf{
	long mesg_type;
	char mesg_text[100];
}message;

key_t read_key,write_key;
int read_msgqid,write_msgqid;


void sigint(int sig)
{
	gets(message.mesg_text);
	message.mesg_type=3;
	msgsnd(write_msgqid,&message,sizeof(message),0);
	printf("in client:: %s\n",message.mesg_text);
}

int main()
{
	read_key=ftok("client",101);
	write_key=ftok("server",102);
	read_msgqid=msgget(read_key,0666 | IPC_CREAT);
	write_msgqid=msgget(write_key,0666 | IPC_CREAT);

	while(1)
	{
		signal(SIGINT,sigint);
		if(msgrcv(read_msgqid,&message,sizeof(message),10,0)!=-1)
			printf("%s\n",message.mesg_text);
	}

}
