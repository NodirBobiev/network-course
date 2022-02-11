#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define PORT 5000
#define MAXLINE 1000

int sockfd;
struct sockaddr_in servaddr;
	

void *receive(void *vargp)
{
	char buffer[100];
	while(1){
		recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
		printf("Server: %s\n", buffer);
	}
	return NULL;
}
int main()
{
	int n;
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_addr.s_addr = inet_addr("192.168.43.16");
	servaddr.sin_port = htons(PORT);
	servaddr.sin_family = AF_INET;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("\n Error : Connect Failed \n");
		exit(0);
	}

	pthread_t receiveT, sendT;
	pthread_create(&receiveT, NULL, receive, NULL);
	
	char msg[100] = "Hello Server";
	sendto(sockfd, msg, MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr));
	
	while(1){
		fgets(msg, 99, stdin);
		sendto(sockfd, msg, MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr));
	}
	close(sockfd);
}
