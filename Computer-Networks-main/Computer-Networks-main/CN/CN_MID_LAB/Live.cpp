#include "cn_header.h"

using namespace std;
int connect_socket(int port_no)
{
    int sfd;
	struct sockaddr_in serv_addr,cli_addr;
	socklen_t cli_len;

	if((sfd = socket(AF_INET,SOCK_STREAM,0))==-1)
	perror("\n socket ");
	else printf("\n socket created successfully");

	bzero(&serv_addr,sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	int opt=1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	if(bind(sfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))==-1)
	perror("\n bind : ");
	else printf("\n bind successful ");

	listen(sfd,10);

	cli_len=sizeof(cli_addr);
	int nsfd;
	if((nsfd = accept(sfd , (struct sockaddr *)&cli_addr , &cli_len))==-1)
	perror("\n accept ");
	else printf("\n accept successful\n");
    return nsfd;
}


int main(int argc, char *argv[])
{
    int nsfd = connect_socket(PORT);
    
    while(1)
    {
		char buf[BUFFER_SIZE];
        read(nsfd, buf, BUFFER_SIZE);
        cout<<"LIVE BROADCAST : ";
        cout<<buf<<"\n";
        break;
    }
}