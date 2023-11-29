#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 8080

int main() {
	int sfd;
	char buf[100];
	char msg[] = "Hello from server";
	struct sockaddr_in s_ad,c_ad;


	if ( (sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
  {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&s_ad, 0, sizeof(s_ad));
	memset(&c_ad, 0, sizeof(c_ad));

	s_ad.sin_family = AF_INET;
	s_ad.sin_addr.s_addr = INADDR_ANY;
	s_ad.sin_port = htons(PORT);

	if ( bind(sfd, (const struct sockaddr *)&s_ad,sizeof(s_ad)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	int len, n;

	len = sizeof(c_ad);

	n = recvfrom(sfd, (char *)buf,100,MSG_WAITALL, ( struct sockaddr *) &c_ad,&len);
	buf[n] = '\0';
	printf("Client : %s\n", buf);
	sendto(sfd, (const char *)msg, strlen(msg),MSG_CONFIRM, (const struct sockaddr *) &c_ad,len);
	printf("Hello message sent.\n");

	return 0;
}
