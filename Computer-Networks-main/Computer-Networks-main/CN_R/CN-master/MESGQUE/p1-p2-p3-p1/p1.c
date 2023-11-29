#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message_buf{
	long mesg_type;
	char mesg_text[100];
}message;

int main()
{
	while(1)
	{
		key_t key=ftok("progfile",65);
		int msgqid=msgget(key,0666 | IPC_CREAT);
		message.mesg_type=1;
		gets(message.mesg_text);
		msgsnd(msgqid,&message,sizeof(message),0);
		if(msgrcv(msgqid,&message,sizeof(message),3,0)!=-1)
			printf("%s\n",message.mesg_text);
	}
}
