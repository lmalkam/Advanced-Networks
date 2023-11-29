#include"../header.h"
int main(int argc, char * argv[])
{
    if (argc != 2)
    {
      printf ("need destination for tracert\n");
      exit (0);
    }
  int sfd = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP);
  char buf[4096] = { 0 };
  struct ip *ip_hdr = (struct ip *) buf;
  int hop = 0;

  int one = 1;
  const int *val = &one;
  if (setsockopt (sfd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
    printf ("Cannot set HDRINCL!\n");

  struct sockaddr_in addr;
  addr.sin_port = htons (7);
  addr.sin_family = AF_INET;
  inet_pton (AF_INET, argv[1], &(addr.sin_addr));


  while (1)
    {
      ip_hdr->ip_hl = 5;
      ip_hdr->ip_v = 4;
      ip_hdr->ip_tos = 0;
      ip_hdr->ip_len = 20+sizeof(icmphdr);
      ip_hdr->ip_id = 10000;
      ip_hdr->ip_off = 0;
      ip_hdr->ip_ttl = hop;
      ip_hdr->ip_p = IPPROTO_ICMP;
      inet_pton (AF_INET, "172.30.228.251", &(ip_hdr->ip_src));
      inet_pton (AF_INET, argv[1], &(ip_hdr->ip_dst));
      ip_hdr->ip_sum = csum ((unsigned short *) buf, sizeof(ip));

      struct icmphdr *icmphd = (struct icmphdr *) (buf + 20);
      icmphd->type = ICMP_ECHO;
      icmphd->code = 0;
      icmphd->checksum = 0;
      icmphd->un.echo.id = 0;
      icmphd->un.echo.sequence = hop + 1;
      icmphd->checksum = csum ((unsigned short *) (buf + 20), sizeof(icmphdr));
      sendto (sfd, buf, sizeof(ip)+sizeof(icmphdr), 0, (sa) & addr, sizeof addr);
      char buff[4096] = { 0 };
      struct sockaddr_in addr2;
      socklen_t len = sizeof (struct sockaddr_in);
      recvfrom (sfd, buff, sizeof(buff), 0, (sa) & addr2, &len);
      struct icmphdr *icmphd2 = (struct icmphdr *) (buff + 20);
      if (icmphd2->type != 0)
    printf ("hop limit:%d Address:%s\n", hop, inet_ntoa (addr2.sin_addr));
      else
    {
      printf ("Reached destination:%s with hop limit:%d\n",
          inet_ntoa (addr2.sin_addr), hop);
      exit (0);
    }

      hop++;
    }

  return 0;
}
