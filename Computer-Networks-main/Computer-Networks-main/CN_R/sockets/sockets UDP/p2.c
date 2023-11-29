 #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 8080
#define MAXLINE 1024

// Driver code
int main() {
	int sfd;
	char buf[100];
	char msg[] = "Hello from client";
	struct sockaddr_in	 s_ad;

	if ( (sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
  {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&s_ad, 0, sizeof(s_ad));

	s_ad.sin_family = AF_INET;
	s_ad.sin_port = htons(PORT);
	s_ad.sin_addr.s_addr = INADDR_ANY;

	int n, len;

	sendto(sfd, (const char *)msg, strlen(msg),MSG_CONFIRM, (const struct sockaddr *) &s_ad,sizeof(s_ad));
	printf("Hello message sent.\n");

	n = recvfrom(sfd, (char *)buf, MAXLINE,MSG_WAITALL, (struct sockaddr *) &s_ad,&len);
	buf[n] = '\0';
	printf("Server : %s\n", buf);

	close(sfd);
	return 0;
}
