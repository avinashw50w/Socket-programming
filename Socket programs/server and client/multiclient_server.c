#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>

struct sockaddr_in server, client;
int sockfd, clientfd;
char buffer[1024];

void* fun(){
	int data_size = 1;
	while((data_size = recv(clientfd, buffer, sizeof(buffer), 0) > 0)){	 	
		printf("Client %s : %s\n", inet_ntoa(client.sin_addr), buffer);
		memset(buffer,'\0',sizeof(buffer));
	}
	if(data_size == 0){
		puts("Client disconnected");
	}
	if(data_size < 0){
		perror("recv falied:");
	}
	pthread_join(pthread_self());
}

int main(int argc, char const *argv[]){
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket:");
		exit(-1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(10000);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&server.sin_zero, sizeof(server.sin_zero));

	socklen_t len = sizeof(struct sockaddr_in);
	if(bind(sockfd, (struct sockaddr*)&server, len) < 0){
		perror("bind:");
		exit(-1);
	}
	if(listen(clientfd,5) < 0){
		perror("listen:");
		exit(-1);
	}
	else puts("Listening...");
	while(1){
		if((clientfd = accept(sockfd, (struct sockaddr*)&client, &len)) < 0){
			perror("accept:");
			exit(-1);
		}
		pthread_id t;
		if(pthread_create(&t, NULL, fun, NULL)){
			perror("pthread:");
			exit(-1);
		}
	}
	return 0;
}
