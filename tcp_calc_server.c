//basically a server but with some parsing abilities implemented.
#include "TCP_CALC.h"
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>


#define MAX 80
#define PORT 10500
#define SA struct sockaddr

void func(int sockfd){
	struct operation op;
	char buf[MAX];
	long answer;
	int n;

	for(;;){
		bzero(buf, MAX);
		read(sockfd, buf, sizeof(buf));
		if(parse_string(buf, &op)== 0){	
			printf("From client: %s Sever reply: ", buf);	
			printf("[!] processed a mathematical operation!\n");
			bzero(buf, MAX);
			switch (op.type){
				case TYPE_ADD:
		        	answer = op.left+op.right;
		        	sprintf(buf, "%ld", answer);
		        	strcat(buf, "\n");
		        	write(sockfd, buf, sizeof(buf));		        	
		        	break;
		    	case TYPE_SUB:
		        	answer = op.left-op.right;
		        	sprintf(buf, "%ld", answer);
		        	strcat(buf, "\n");
		        	write(sockfd, buf, sizeof(buf));
		        	break;
		    	case TYPE_MUL:
		        	answer = op.left*op.right;
		        	sprintf(buf, "%ld", answer);
		        	strcat(buf, "\n");
		        	write(sockfd, buf, sizeof(buf));
		        	break;
		    	case TYPE_DIV:
		        	answer = op.left/op.right;
		        	sprintf(buf, "%ld", answer);
		        	strcat(buf, "\n");
		        	write(sockfd, buf, sizeof(buf));
		        	break;
			}	
		}else{
			printf("From client: %s Sever reply: ", buf);
			bzero(buf, MAX);
			n = 0;
		
			while((buf[n++] = getchar()) != '\n');
				;
			write(sockfd, buf, sizeof(buf));
		}
		if(strncmp("exit", buf, 4) == 0){
			printf("[+] server called exit. Terminating.\n");
			break;
		}
	}
}


int main(){
	int sockfd, connfd, len;
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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0){
		printf("[-] socket bind failed.\n");
		exit(0);
	}else{
		printf("[+] socket successfully binded.\n");
	}

	if((listen(sockfd, 5)) != 0){
		printf("[-] listen failed.\n");
		exit(0);
	}else{
		printf("[+] server listening.\n");
	}
	len = sizeof(cli);

	connfd = accept(sockfd, (SA*)&cli, &len);
	if(connfd < 0){
		printf("[-] server could not accept the connection from client.\n");
		exit(0);
	}else{
		printf("[+] server accepted the connection request.\n");
		printf("[+] waiting for client.\n ");
	}
	func(connfd);
	close(sockfd);
}
