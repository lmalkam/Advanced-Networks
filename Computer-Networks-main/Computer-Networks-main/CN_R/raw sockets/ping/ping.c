#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stddef.h>
#include <fcntl.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <time.h>


#define h_addr h_addr_list[0]

unsigned short chksum(void *b, int len) {    
    unsigned short *buf = b; 
    unsigned int sum = 0; 
    unsigned short result; 

    for ( sum = 0; len > 1; len -= 2 ) 
        sum += *buf++; 
    if ( len == 1 ) 
        sum += *(unsigned char*)buf; 
    sum = (sum >> 16) + (sum & 0xFFFF); 
    sum += (sum >> 16); 
    result = ~sum; 
    return result; 
}

#define PACKET_SIZE 64
int flag = 1;

void hdfn(int signo) {
    flag = 0;
}

struct ping_pkt{
    struct icmphdr hdr;
    char *msg;
};

void pinger(int rsfd, struct sockaddr_in ping_addr, char *ping_ip, char *ping_domain_name, char *domain) {
    printf("PING %s (%s)\n", domain, ping_ip);

    int ttl = 100, msgs_sent = 0, msgs_recvd = 0, len;
    struct timespec tts, tte, st, et;
    long double rtt_msec = 0, total_msec = 0;
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    struct ping_pkt pkt;
    struct sockaddr_in r_addr;

    if(setsockopt(rsfd, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
        perror("setsockopt-ttl");
        exit(1);
    }

    if(setsockopt(rsfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv)) < 0) {
        perror("setsockopt-recv-timeout");
        exit(1);
    }

    pid_t pid = getpid();
    int pkt_msg_len = PACKET_SIZE - sizeof(struct icmphdr);

    while(flag) {
        //initialize packet
        bzero(&pkt, sizeof(pkt));
        pkt.hdr.type = ICMP_ECHO;
        pkt.hdr.un.echo.id = pid;
        pkt.msg = (char*)malloc(pkt_msg_len);
        for(int i = 0; i < pkt_msg_len - 1; i++) {
            pkt.msg[i] = i + '0';
        }
        pkt.msg[pkt_msg_len - 1] = 0;
        pkt.hdr.un.echo.sequence = msgs_sent++;
        pkt.hdr.checksum = chksum(&pkt, sizeof(pkt));

        //send packet
        int packet_sent = 1;
        clock_gettime(CLOCK_MONOTONIC, &st);
        if(sendto(rsfd, &pkt, sizeof(pkt), 0, (struct sockaddr*)&ping_addr, sizeof(ping_addr)) < 0) {
            packet_sent = 0;
            printf("Packet sending failed\n");
        }
        else {
            //recieve packet
            char buff[NI_MAXHOST];
            len = sizeof(r_addr);
            if(recvfrom(rsfd, buff, sizeof(buff), 0, (struct sockaddr*)&r_addr, &len) < 0) {
                printf("Packet recieving failed\n");
            }

            else {
                //calculate times
                clock_gettime(CLOCK_MONOTONIC, &et);
                rtt_msec = (long double)(et.tv_nsec - st.tv_nsec)/1000000.0;
                rtt_msec += (long double)(et.tv_sec - st.tv_sec)*1000;
                total_msec += rtt_msec;
                
                struct icmphdr *h = (struct icmphdr*)(buff + sizeof(struct iphdr));
                if(h->type != 0) {
                    printf("Error packet recieved with type %d and code %d\n", h->type, h->code);
                }
                else {
                    printf("%d bytes from %s (%s) : icmp_seq = %d time = %Lf\n", PACKET_SIZE, ping_domain_name, ping_ip, msgs_sent, rtt_msec);
                    msgs_recvd++;
                }
            }
        }
        sleep(1);
    }
    printf("\n--- %s ping statistics ---\n", domain);
    double packet_loss = ((msgs_sent - msgs_recvd)/msgs_sent)*100;
    printf("%d packets sent, %d packets recieved, %f%c packet loss, time %Lfms\n", msgs_sent, msgs_recvd, packet_loss, '%', total_msec);
    printf("rtt avg = %Lf\n", total_msec/msgs_sent);
}

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("Usage -\n ./ping <domain-name>");
        exit(1);
    }

    struct sockaddr_in ping_addr;
    //get domain details
    struct hostent *ping_details = gethostbyname(argv[1]);
    if(ping_details == NULL) {
        printf("Could not find ip-address associated with the domain name\n");
        exit(1);
    }

    //fill addr
    bcopy((char *) ping_details->h_addr, 
         (char *) &ping_addr.sin_addr.s_addr, 
         ping_details->h_length);
    ping_addr.sin_family = AF_INET;
    ping_addr.sin_port = htons(0);

    //convert to required formats
    char ping_ip[1025], ping_domain_name[1025];
    strcpy(ping_ip, inet_ntoa(*(struct in_addr *)ping_details->h_addr));
    int len = sizeof(struct sockaddr_in);
    if(getnameinfo((struct sockaddr *)&ping_addr, len, ping_domain_name,  sizeof(ping_domain_name), NULL, 0, NI_NAMEREQD) < 0)  { 
        printf("Could not resolve reverse lookup of hostname\n"); 
        exit(1);
    }
    
    //signal handler for termination
    signal(SIGINT, hdfn);

    int rsfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(rsfd < 0) {
        perror("rsfd");
        exit(1);
    }

    pinger(rsfd, ping_addr, ping_ip, ping_domain_name, argv[1]);
}


