#include <sys/socket.h>
#include <error.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_LENGTH 1024

int main(){
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(2048);
	
	int name_fd = bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr));
	if(name_fd == -1){
		perror("bind error");
		return -1;
	}
	
	name_fd = listen(sockfd, 10);
	
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);
	int clientfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
	
	while(1){  // while循环可以多次接收客户端发送的数据
		char buffer[BUFFER_LENGTH] = {0};
		int ret = recv(clientfd, buffer, BUFFER_LENGTH, 0);
		if(ret == 0){
			close(clientfd);
			break;
		}
		printf("%s\n",buffer);
		send(clientfd, buffer, ret, 0);
	}
	return 0;
}

