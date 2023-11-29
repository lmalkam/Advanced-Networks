#include "tcp_header.h"

using namespace std;


void extract_packet( char *buffer, size_t length, FILE *log_file, int turn);
void save_data_to_file( char *buffer, size_t length, FILE *log_file);
void print_header_to_file(struct tcphdr *tcp_header, FILE *log_file);

int main(int argc, char **argv) {
    int rsfd = 0, buffer_size = 0; 
    char *buffer = ( char*)malloc(PACKET_SIZE);

    if(argc != 3) {
        printf("Usage: %s <LOG FILE NAME> <No of Packets : integer>\n", argv[0]);
        return 1;
    }

    FILE *log_file = fopen(argv[1], "w");
    if(log_file == NULL) 
    {
        perror("Opening in Log File");
        return 1;
    }

    rsfd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(rsfd == -1) 
    {
        perror("Creating Raw Socket Failed");
        return 1;
    }

    cout<<"Transferring TCP Header to our log file: "<<argv[1]<<"\n";
    int turn = 1;
    while(turn < atoi(argv[2])+1)
    {
        buffer_size = recvfrom(rsfd, buffer, PACKET_SIZE, 0, NULL, NULL);
        if(buffer_size == -1) {
            perror("Unable to retrieve data from socket");
            return 1;
        }
        extract_packet(buffer, buffer_size, log_file,turn++);
        cout<<">> Successfully extracted packet "<<turn-1<<" and saved it to "<<argv[1]<<endl;
    }

    return 0;
}


void extract_packet(char *buffer, size_t length, FILE *log_file, int turn) {
    fprintf(log_file, "\n\n>>>>>>>>>>>>>>>>>>> TCP PACKET %d <<<<<<<<<<<<<<<<<<<",turn);

    struct iphdr *ip_header = (struct iphdr*)buffer;
    unsigned short ip_header_len = ip_header->ihl*4;
    static int packet_count = 0;

    // Print TCP header
    struct tcphdr *tcp_header = (struct tcphdr*)(buffer + ip_header_len);

    fprintf(log_file, "\nTCP HEADER:\n");
    
    print_header_to_file(tcp_header, log_file);

    fprintf(log_file, "\n\n############## DATA ##############\n");
    fprintf(log_file, "TCP HEADER DATA\n");
    save_data_to_file(buffer + ip_header_len, tcp_header->doff*4, log_file);

    int payload_len = (length - tcp_header->doff*4 - ip_header->ihl*4);
    if(payload_len > 0)
    {
        fprintf(log_file, "Payload DATA\n");
        save_data_to_file(buffer + ip_header_len + tcp_header->doff*4, payload_len, log_file);
    }
    fprintf(log_file, "\n\n==============================================================================================================\n");
}

void print_header_to_file(struct tcphdr *tcp_header, FILE *log_file)
{
    fprintf(log_file, "   Source port             : %u\n", ntohs(tcp_header->source));
    fprintf(log_file, "   Destination port        : %u\n", ntohs(tcp_header->dest));
    fprintf(log_file, "   Sequence number         : %u\n", ntohl(tcp_header->seq));
    fprintf(log_file, "   Acknowledgement number  : %u\n", ntohl(tcp_header->ack_seq));
    fprintf(log_file, "   Header length           : %u Bytes\n", (unsigned int)tcp_header->doff*4);
    fprintf(log_file, "   URG_FLAG                : %u\n", (unsigned int)tcp_header->urg);
    fprintf(log_file, "   ACK_FLAG                : %u\n", (unsigned int)tcp_header->ack);
    fprintf(log_file, "   RST_FLAG                : %u\n", (unsigned int)tcp_header->rst);
    fprintf(log_file, "   SYN_FLAG                : %u\n", (unsigned int)tcp_header->syn);
    fprintf(log_file, "   FIN_FLAG                : %u\n", (unsigned int)tcp_header->fin);
    fprintf(log_file, "   PSH_FLAG                : %u\n", (unsigned int)tcp_header->psh);
    fprintf(log_file, "   Window Size             : %u\n", htons(tcp_header->window));
    fprintf(log_file, "   Checksum                : %u\n", htons(tcp_header->check));
    fprintf(log_file, "   Urgent Pointer          : %u\n", htons(tcp_header->urg_ptr));
}

void save_data_to_file(char *buffer, size_t length, FILE *log_file) 
{
    for(int i = 0; i < length; i++) 
    {
        if(i != 0 && i % 16 == 0) 
        {
            fprintf(log_file, "          ");
            for(int j = (i-16); j < i; j++) 
            {
                if(buffer[j] >= 32 && buffer[j] <= 128) 
                    fprintf(log_file, "%c", (unsigned char)buffer[j]);
                else 
                    fprintf(log_file, "."); 
            }
            fprintf(log_file, "\n");
        }

        if(i%16==0)
            fprintf(log_file, "    ");
        fprintf(log_file, " %02X", (unsigned int)buffer[i]);

        if(i == (length-1)) 
        {
            for(int j = 0; j < (15-1%16); j++)
                fprintf(log_file, "    ");

            fprintf(log_file, "          ");

            for(int j=(i-i%16); j <= 1; j++) 
            {
                if(buffer[j] >= 32 && buffer[j] <= 128)
                    fprintf(log_file, "%c", (unsigned char)buffer[j]);
                else
                    fprintf(log_file, ".");
            }

            fprintf(log_file, "\n");
        }
    }
}
