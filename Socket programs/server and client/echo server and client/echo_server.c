#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

struct sockaddr_in server, client;
int socketfd, clientfd;
char buffer[1024];

int main(int argc, char const *argv[]){
	if(argc < 2){
		perror("Usage: argv[1] = port no.\n");
		exit(-1);
	}
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("Socket :");
		exit(-1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&server.sin_zero,8);

	socklen_t len = sizeof(sockaddr_in);
	if(bind(sockfd, (struct sockaddr*)&server, len) == -1){
		perror("Bind :");
		exit(-1);
	}
	if(listen(sockfd,5) == -1){
		perror("Listen :");
		exit(-1);
	}
	while(1) {
		if((clientfd = accept(sockfd, (struct sockaddr*)&client, &len)) == -1){
			perror("accept :");
			exit(-1);
		}
		int data_size = 1;
		while(data_size){
			data_size = recv(clientfd, buffer, sizeof(buffer), 0);
			if(data_size){
				buffer[data_size] = '\0';
				printf("client %s : %s\n", inet_ntoa(client.sin_addr), buffer);
			}
		}
		printf("Client disconnected...\n");
		close(clientfd);
	}

	return 0;
}
