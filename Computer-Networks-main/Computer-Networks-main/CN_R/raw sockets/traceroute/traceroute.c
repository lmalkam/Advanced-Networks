#include <myHead.h>
#include <time.h>

#define PACKET_SIZE 64

struct trace_pkt{
    struct icmphdr hdr;
    char *msg;
};

void tracer(int rsfd, struct sockaddr_in trace_addr, char *trace_ip, char *trace_domain_name, char *domain) {
    int max_hops = 30, curr_hops = 1, sequence = 0, len;
    printf("traceroute to %s((%s) (%s)), %d hops max, %d packet size\n", domain, trace_domain_name, trace_ip, max_hops, PACKET_SIZE);

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    struct trace_pkt pkt;
    struct sockaddr_in r_addr;


    if(setsockopt(rsfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv)) < 0) {
        perror("setsockopt-recv-timeout");
        exit(1);
    }

    pid_t pid = getpid();
    int pkt_msg_len = PACKET_SIZE - sizeof(struct icmphdr);
    while(curr_hops <= max_hops) {
        //increase ttl at every iteration
        if(setsockopt(rsfd, SOL_IP, IP_TTL, &curr_hops, sizeof(curr_hops)) < 0) {
            perror("setsockopt-ttl");
            exit(1);
        }

        //initialize packet
        bzero(&pkt, sizeof(pkt));
        pkt.hdr.type = ICMP_ECHO;
        pkt.hdr.un.echo.id = pid;
        pkt.msg = (char*)malloc(pkt_msg_len);
        for(int i = 0; i < pkt_msg_len - 1; i++) {
            pkt.msg[i] = i + '0';
        }
        pkt.msg[pkt_msg_len - 1] = 0;
        pkt.hdr.un.echo.sequence = sequence++;
        pkt.hdr.checksum = chksum(&pkt, sizeof(pkt));

        //send packet
        if(sendto(rsfd, &pkt, sizeof(pkt), 0, (struct sockaddr*)&trace_addr, sizeof(trace_addr)) < 0) {
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
                //extract icmp header
                struct icmphdr *h = (struct icmphdr*)(buff + sizeof(struct iphdr));

                char ip_addr[NI_MAXHOST], ip_domain[NI_MAXHOST];
                struct in_addr in;
                in.s_addr = r_addr.sin_addr.s_addr;
                strcpy(ip_addr, inet_ntoa(in));
                len = sizeof(struct sockaddr_in);
                if(getnameinfo((struct sockaddr *)&r_addr, len, ip_domain,  sizeof(ip_domain), NULL, 0, NI_NAMEREQD) < 0) {
                    printf("%d %s (%s)\n", curr_hops, ip_addr, ip_addr);
                }
                else {
                    printf("%d %s (%s)\n", curr_hops, ip_domain, ip_addr);
                }
                
                //check if destination reached (i.e got reply)
                if(h->type == 11 && h->code == 0) {
                    curr_hops++;
                }
                else {
                    curr_hops = max_hops + 1;
                }
                
            }
        }
        sleep(1);
    }
}

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("Usage -\n ./ping <domain-name>");
        exit(1);
    }

    struct sockaddr_in trace_addr;
    //get domain details
    struct hostent *trace_details = gethostbyname(argv[1]);
    if(trace_details == NULL) {
        printf("Could not find ip-address associated with the domain name\n");
        exit(1);
    }

    //fill addr
    bcopy((char *) trace_details->h_addr, 
         (char *) &trace_addr.sin_addr.s_addr, 
         trace_details->h_length);
    trace_addr.sin_family = AF_INET;
    trace_addr.sin_port = htons(0);

    //convert to required formats
    char trace_ip[1025], trace_domain_name[1025];
    strcpy(trace_ip, inet_ntoa(*(struct in_addr *)trace_details->h_addr));
    int len = sizeof(struct sockaddr_in);
    if(getnameinfo((struct sockaddr *)&trace_addr, len, trace_domain_name,  sizeof(trace_domain_name), NULL, 0, NI_NAMEREQD) < 0)  { 
        printf("Could not resolve reverse lookup of hostname\n"); 
        exit(1);
    }

    int rsfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(rsfd < 0) {
        perror("rsfd");
        exit(1);
    }

    tracer(rsfd, trace_addr, trace_ip, trace_domain_name, argv[1]);

}
