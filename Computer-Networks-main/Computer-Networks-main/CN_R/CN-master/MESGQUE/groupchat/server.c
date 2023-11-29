#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/unistd.h>

struct message_buf{
	long mesg_type;
	char mesg_text[100];
};

key_t read_key,write_key;
int read_msgqid,write_msgqid;
int total_client=0;
long pids[100];
long groups[10][100];
int grp_count[]={0,0,0,0,0,0,0,0,0,0};
char grp_no[]={'0','1','2','3','4','5','6','7','8','9'};

void is_there(long pid)
{
	for(int i=0;i<total_client;i++)
		if(pids[i]==pid)
			return;

	pids[total_client++]=pid;
	struct message_buf message_write;
	message_write.mesg_type=pid+1;
	strcpy(message_write.mesg_text,"101");
	msgsnd(read_msgqid,&message_write,sizeof(message_write),0);
}

void delete_server(int sig)
{
		msgctl(read_msgqid, IPC_RMID, NULL);
		msgctl(write_msgqid, IPC_RMID, NULL);
}


void add_grp_mem(long pid)
{
	struct message_buf message_write,message_read;
	message_write.mesg_type=pid+1;
	strcpy(message_write.mesg_text,"Hi there you can write down the groups you wanna join");
	msgsnd(read_msgqid,&message_write,sizeof(message_write),0);

	message_read.mesg_type=0;
	int a=msgrcv(read_msgqid,&message_read,sizeof(message_read),pid,0);
	printf("%s\n",message_read.mesg_text);
	if(a!=-1)
	{
		for(int i=0;i<strlen(message_read.mesg_text);i++)
		{
			int a=(int)(message_read.mesg_text[i])-48;
			if(a>0&&a<10)
				groups[a][grp_count[a]++]=pid;
		}
	}

	message_write.mesg_type=pid+1;
	strcpy(message_write.mesg_text,"You have successfull joined all the groups");
	msgsnd(read_msgqid,&message_write,sizeof(message_write),0);
}

int main()
{
	read_key=ftok("server",102);
	write_key=ftok("client",101);
	read_msgqid=msgget(read_key,0666 | IPC_CREAT);
	write_msgqid=msgget(write_key,0666 | IPC_CREAT);

	while(1)
	{
		struct message_buf message,message_write;
		message.mesg_type=0;
		if(msgrcv(read_msgqid,&message,sizeof(message),message.mesg_type,0)!=-1)
		{
			is_there(message.mesg_type);
			if(strcmp(message.mesg_text,"join")==0)
				add_grp_mem(message.mesg_type);

			else
			{
				int ind;
				for(int i=0;i<10;i++)
				{
					if(grp_no[i]==message.mesg_text[0])
					{ind=i;break;}
				}
				//char buff[10];sprintf(buff,"%ld",message.mesg_type);printf("%s\n",buff );
				//strcpy(message_write.mesg_text,buff);
				//printf("%s\n",message_write.mesg_text );
				for(int i=0;i<grp_count[ind];i++)
				{
					 if(groups[ind][i]==message.mesg_type)
					 	continue;
					printf("Sending to this client:: %ld %s\n",groups[ind][i],message.mesg_text);
					struct message_buf message_write;
					message_write.mesg_type=groups[ind][i];
					strcat(message_write.mesg_text,message.mesg_text);
					msgsnd(write_msgqid,&message_write,sizeof(message_write),0);
				}
			}
			signal(SIGINT,delete_server);
		}
	}

}
