// Socket program for server end for TCP(connection oriented) //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

struct sockaddr_in server, client;
int sockfd, clientfd;

int main(int argc, char const *argv[]) {
	/* create the socket */
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket: ");
		exit(-1);
	}
	/* assign IP address and port no. to the structure named server */
	server.sin_family = AF_INET;
	server.sin_port = htons(10000);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&server.sin_zero, sizeof(server.sin_zero)); // server.sin_zero is of 8 bytes //

	socklen_t len = sizeof(struct sockaddr_in);
	/* bind the IP address and port no. assigned to the server to the socket */
	if(bind(sockfd, (struct sockaddr*)&server, len) == -1){
		perror("bind: ");
		exit(-1);
	}
	/* listen for any incoming connection with maximum backlog 5 */
	if(listen(sockfd, 5) == -1){
		perror("listen: ");
		exit(-1);
	}
	else printf("listening\n");
	while(1){
		/* accept the connection request from the client,
		 * otherwise, wait until any client wants to connect to the server */
		clientfd = accept(sockfd, (struct sockaddr*)&client, &len)
		if(clientfd == -1){
			perror("accept: ");
			exit(-1);
		}
		else 
			printf("Connected to a client with IP address %s and port no %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
	
		char msg[] = "Hello World !!!";
		/* send message to the client */
		int sent_bytes = send(clientfd, msg, strlen(msg), 0);
		printf("Sent %d bytes of data to the client\n", sent_bytes);
		/* close the connection */
		close(clientfd);
	}
	
	return 0;
}