#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//For Server
#include <netdb.h>

//For Client
#define PORT 5000
#define BUF_SIZE 1024

int client(const char* filename){
	FILE* fp = fopen(filename, "ab");
	if(fp == NULL){
		printf("[-] ERROR OPENING FILE.\n");
		exit(EXIT_FAILURE);
	}
	printf("[+] CLIENT FILE OPENED SUCCESSFULLY!\n");

	int sockfd = 0;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("[-] COULD NOT CREATE A SOCKET.\n");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))< 0){
		printf("[-] CONNECTION TO SERVER FAILED.\n");
		exit(EXIT_FAILURE);
	}

	int bytesReceived = 0;
	char buff[BUF_SIZE];
	memset(buff, '0', sizeof(buff));
	while((bytesReceived = read(sockfd, buff, BUF_SIZE)) > 0){
		printf("[*] BYTES RECEIVED: %d\n", bytesReceived);
		fwrite(buff, 1, bytesReceived, fp);
	}
	if(bytesReceived < 0){
		printf("[-] READ ERROR.\n");
	}
	return 0;
}

int server(const char* filename){
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("[+] SOCKET RETRIEVE SUCCESS.\n");

	struct sockaddr_in serv_addr;
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT);

	bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

	if(listen(listenfd, 10) == -1){
		printf("[-] FAILED TO LISTEN\n");
		exit(EXIT_FAILURE);
	}
	for(;;){
		int connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
		FILE *fp = fopen(filename, "rb");
		if(fp == NULL){
			printf("[-] COULD NOT OPEN FILE FROM CLIENT SIDE.\n");
			exit(EXIT_FAILURE);
		}
		//Read data from file and send it.
		//printf("[+] TOTAL FILE SIZE: %zd\n",ftell(fp));
		for(;;){
			unsigned char buff[BUF_SIZE] = {0};
			int nread = fread(buff, 1, BUF_SIZE, fp);
			printf("[+] BYTES READ: %d\n", nread);

			if(nread > 0){
				printf("[+] SENDING...\n");
				write(connfd, buff, nread);
			}
			if(nread < BUF_SIZE){
				if(feof(fp)){
					printf("[-] REACHED END OF FILE.\n");
					return 1;
				}
				if(ferror(fp)){
					printf("[-] ERROR READING.\n");
					return -1;
				}
				break;
			}
		}
		close(connfd);
		sleep(2);
	}
	return 0;
}

int main(int argc, char** argv){
	if(argc == 3){
		const char* mode = argv[1];
		const char* filename = argv[2];

		if(strcmp(mode, "--client") == 0){
			return client(filename);
		}else if(strcmp(mode, "--server") == 0){
			return server(filename);
		}else{
			printf("[-] Invalid Mode: %s should be '--client' or '--server'\n", mode);		
		}
	}else{
		printf("[-] Invalid number of argument, usage is: %s [MODE] [FILENAME]\n", argv[0]);
	}
	return 1;
}
