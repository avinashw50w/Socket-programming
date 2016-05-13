#include <stdio.h> 
#include <stdlib.h>    
#include <string.h>    
#include <netinet/tcp.h>   
#include <netinet/ip.h>    
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <arpa/inet.h>

void sniff_tcp_packet(unsigned char *packet);
voif sniff_ip_packet(unsigned char *packet);

FILE *log;
struct sockaddr saddr;
struct sockaddr_in source, dest;
int sockfd;

int main(int argc, char const *argv[]){
	if((log = fopen("log.txt", "w")) == NULL){
		perror("log file:");
		exit(-1);
	}
	if((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0){
		perror("socket:");
		exit(-1);
	}
	socklen_t len = sizeof(saddr);
	int packet_size = sizeof(struct iphdr) + sizeof(struct tcphdr);
	unsigned char *packet = (unsigned char*)malloc(packet_size);

	int data_size = recvfrom(sockfd, packet, packet_size, 0, &saddr, &len);
	if(data_size < 0){
		perror("recvfrom:");
		exit(-1);
	}

	sniff_tcp_packet(packet);
	close(sockfd);
	free(packet);
	return 0;
}

void sniff_ip_packet(unsigned char *packet){
	struct iphdr *ip = (struct iphdr*)packet;

	fprintf(log, "IP Header\n");
	fprintf(log, "  IP Version          	: %d\n", (unsigned int)ip->version);
	fprintf(log, "  IP Header Length    	: %d\n", (unsigned int)ip->ihl);
	fprintf(log, "  Type of Service     	: %d\n", (unsigned int)ip->tos);
	fprintf(log, "  Total Length(in bytes)  : %d\n", ntohs(ip->tot_len)*4);
	fprintf(log, "  Identification		: %d\n", ntohs(ip->id));
	fprintf(log, "  TTL 			: %d\n", (unsigned int)ip->ttl);
	fprintf(log, "  Protocol		: %d\n", (unsigned int)ip->protocol);
	fprintf(log, "  Checksum		: %d\n", ntohs(ip->check));
	fprintf(log, "  Source IP 		: %s\n", inet_ntoa(ip->saddr));
	fprintf(log, "  Destination IP		: %s\n", inet_ntoa(ip->daddr));
}

void sniff_tcp_packet(unsigned char *packet){
	struct iphdr *ip = (struct iphdr*)packet;
	unsigned short iphdrlen = ip->ihl*4;

	struct tcphdr *tcp = (struct tcphdr*)(packet + iphdrlen);   
         
    print_ip_header(packet);
         
    fprintf(log, "\n");
    fprintf(log, "TCP Header\n");
    fprintf(log, "   Source Port             	: %u\n", ntohs(tcp->source));
    fprintf(log, "   Destination Port        	: %u\n", ntohs(tcp->dest));
    fprintf(log, "   Sequence Number         	: %u\n", ntohl(tcp->seq));
    fprintf(log, "   Acknowledge Number      	: %u\n", ntohl(tcp->ack_seq));
    fprintf(log, "   Header Length(in bytes)    : %d\n", (unsigned int)tcp->doff*4);
    fprintf(log, "   Urgent Flag             	: %d\n", (unsigned int)tcp->urg);
    fprintf(log, "   Acknowledgement Flag    	: %d\n", (unsigned int)tcp->ack);
    fprintf(log, "   Push Flag               	: %d\n", (unsigned int)tcp->psh);
    fprintf(log, "   Reset Flag              	: %d\n", (unsigned int)tcp->rst);
    fprintf(log, "   Synchronise Flag        	: %d\n", (unsigned int)tcp->syn);
    fprintf(log, "   Finish Flag             	: %d\n", (unsigned int)tcp->fin);
    fprintf(log, "   Window                  	: %d\n", ntohs(tcp->window));
    fprintf(log, "   Checksum                	: %d\n", ntohs(tcp->check));
    fprintf(log, "   Urgent Pointer          	: %d\n", tcp->urg_ptr);
    fprintf(log, "\n\n\n");
                      
}
