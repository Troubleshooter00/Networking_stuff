#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 10500
#define SA struct sockaddr

void func(int sockfd){
	char buf[MAX];
	int n;
	for(;;){
		bzero(buf, sizeof(buf));
		printf("client: ");
		n = 0;
		while((buf[n++] = getchar()) != '\n')
			;
		write(sockfd, buf, sizeof(buf));
		bzero(buf, sizeof(buf));
		read(sockfd, buf, sizeof(buf));
		printf("server: %s", buf);
		if(strncmp(buf, "exit", 4) == 0){
			printf("[+] server called exit.");
			break;
		}
	}
}


int main(){
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		printf("[-] socket creation failed.\n");
		exit(0);
	}else{
		printf("[+] socket created successfully.\n");
	}
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	if(connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0){
		printf("[-] connection with the server failed.\n");
		exit(0);
	}else{
		printf("[+] connected to the server.\n");
	}
	func(sockfd);
	close(sockfd);
}
