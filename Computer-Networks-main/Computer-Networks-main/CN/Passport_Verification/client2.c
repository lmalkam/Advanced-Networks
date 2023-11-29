#include "cn_header.h"

#define PORT 3080
#define SIZE 512
#define SERVICE 3

int sfd, rsfd;
int loc = 0, addrlen ;
struct sockaddr_in server_addr ;
char documents[3][SIZE] ;
char DOCTYPE[3][10] = {
	"AADHAR",
	"SSC",
	"PAN"
};

char DOCNUM[3][15] = {
	"123456789012",
	"87654321",
	"DIRPA123"
};

struct msg_buf {
    long type ;
    char text[100] ;
} ;

/**
 * using descriptor
 * 1 : AADHAR CARD No.
 * 2 : SSC No.
 * 3 : PAN CARD No.
 */

void recv_utils()
{
	char buffer[SIZE];
	recvfrom(rsfd,buffer,SIZE,0,NULL,NULL);

	struct iphdr* ip_header;
	ip_header=(struct iphdr*)buffer;

	printf("%s\n",buffer+(ip_header->ihl)*4);

	for(int i=0; i < SERVICE; i++)
	{
		if( strcmp(buffer, DOCTYPE[i]) == 0 )
		{
			strcpy(documents[loc],DOCNUM[i]);
			break;
		}
	}

	loc = (loc + 1) % SERVICE;
}

int main() {
	int msg_ID = msgget((key_t)ftok("msg.txt", 'b'), 0777) ;
	struct msg_buf r_msg ;
	rsfd = socket (AF_INET, SOCK_RAW, 2);
	for(int i = 0; i < SERVICE; i++)
		recv_utils() ;

	// connecting SOCKET
	server_addr.sin_family = AF_INET ;
    server_addr.sin_port = htons(PORT) ;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;
    addrlen = sizeof(server_addr) ;
    
    sfd = socket(AF_INET, SOCK_STREAM, 0) ;
    connect(sfd, (struct sockaddr *)&server_addr, addrlen) ;

	for(int i = 0; i < SERVICE; i++) 
	{
		send(sfd, documents[i], strlen(documents[i]), 0) ;
		int response_return = msgrcv(msg_ID, &r_msg, sizeof(r_msg), 3, 0) ;
		printf("response return =%d \nMessage[received]=%s\n", response_return, r_msg.text) ;
	}
        
}
