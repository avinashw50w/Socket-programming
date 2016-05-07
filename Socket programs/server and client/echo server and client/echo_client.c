#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

struct sockaddr_in server;
int clientfd;
char buffer[1024];
int main(int argc, char const *argv[]){
	if(argc < 3){
		perror("Usage: argv[1] = IP address , argv[2] = Port number\n");
		exit(-1);
	}
	if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket:");
		exit(-1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);
	bzero(&server.sin_zero, sizeof(server.sin_zero));

	if(connect(clientfd, (struct sockaddr*)&server, sizeof(server)) == -1){
		perror("connect:");
		exit(-1);
	}
	while(1){
		memset(buffer,'\0',sizeof(buffer));
		fgets(buffer,sizeof(buffer),stdin);
		send(clientfd, buffer, strlen(buffer), 0);
	}
	close(clientfd);
	return 0;
}