#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct message_buf{
	long mesg_type;
	char mesg_text[100];
};

key_t read_key,write_key;
int read_msgqid,write_msgqid;;

int main()
{
	read_key=ftok("server",102);
	write_key=ftok("client",101);
	read_msgqid=msgget(read_key,0666 | IPC_CREAT);
	write_msgqid=msgget(write_key,0666 | IPC_CREAT);

	while(1)
	{
		struct message_buf message,message_write;
		if(msgrcv(read_msgqid,&message,sizeof(message),message.mesg_type,0)!=-1)
		{
			printf("in server:: %s %ld\n",message.mesg_text,message.mesg_type);
			if(message.mesg_type==2)
			{
				message_write.mesg_type=15;
				strcpy(message_write.mesg_text,message.mesg_text);
				msgsnd(write_msgqid,&message_write,sizeof(message_write),0);
			}
			else if(message.mesg_type==3)
			{
				message_write.mesg_type=10;
				strcpy(message_write.mesg_text,message.mesg_text);
				msgsnd(write_msgqid,&message_write,sizeof(message_write),0);
			}
			else if(message.mesg_type==5)
			{
				message_write.mesg_type=6;
				strcpy(message_write.mesg_text,message.mesg_text);
				msgsnd(write_msgqid,&message_write,sizeof(message_write),0);
			}
		}
	}

}
