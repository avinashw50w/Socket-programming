// Socket program for client end for TCP(connection oriented) //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

struct sockaddr_in server, client;
int sockfd;
char buff[1024];
int main(int argc, char const *argv[]){
	if(argc < 3){
		perror("Usage : argv[1] = IP address , argv[2] = Port no.\n");
		exit(-1);
	}
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket :");
		exit(-1);
	}
	/* assign the IP address and port no. of the server to which  
	 * it wants to connect */
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);
	bzero(&server.sin_zero,8);

	socklen_t len = sizeof(serveraddr_in);
	/* connect to the server */
	if(connect(sockfd, (struct sockaddr*)&server, len) == -1){
		perror("connect :");
		exit(-1);
	}
	/* recieve data from the server */
	int recv_bytes = recv(sockfd, buff, 1024, 0);
	buff[recv_bytes] = '\0';

	printf("%s\n", buff);
	
	close(sockfd);

	return 0;
}
