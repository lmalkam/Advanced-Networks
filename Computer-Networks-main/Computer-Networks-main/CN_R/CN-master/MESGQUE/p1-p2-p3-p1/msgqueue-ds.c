#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

int main(){
  int key = ftok("/Desktop/3-2/CN/CN_Codes/MESGQUE/p1-p2-p3-p1",0);
  int msgid = msgget(key, 0666|IPC_CREAT);
  struct msqid_ds buf;
  if(msgctl(msgid,IPC_STAT,&buf)==-1) return 0;
  printf("Time of last msgsnd: %ld\n",buf.msg_stime);
  printf("Time of last msgrcv: %ld\n",buf.msg_rtime);
  printf("Time of last change: %ld\n",buf.msg_ctime);
  printf("No. of bytes in msgqueue: %ld\n",buf.__msg_cbytes);
  printf("Current no. of messages in queue: %ld\n",buf.msg_qnum);
  printf("Max number of bytes allowed in queue: %ld\n",buf.msg_qbytes);
  printf("PID of last msgsnd: %d\n",buf.msg_lspid);
  printf("PID of last msgrcv: %d\n",buf.msg_lrpid);
  return 0;
}
