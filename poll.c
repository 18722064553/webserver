#include <sys/socket.h>
#include <error.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/poll.h>

#define BUFFER_LENGTH 1024

#define POLL_SIZE 1024

int main(){

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(struct sockaddr_in));
		
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(2048);
		
	if (-1 == bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr))) {
		perror("bind");
		return -1;
	}


	int name_fd = listen(sockfd, 10);

	
	struct pollfd fds[1024] = {0};
	
		fds[sockfd].fd = sockfd;
		fds[sockfd].events = POLLIN;
	
		int maxfd = sockfd;
	
		while (1) {
	
			int nready = poll(fds, maxfd+1, -1);
	
			if (fds[sockfd].revents & POLLIN) {
	
				struct sockaddr_in clientaddr;
				socklen_t len = sizeof(clientaddr);
				
				int clientfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
	
				printf("sockfd: %d\n", clientfd);
				fds[clientfd].fd = clientfd;
				fds[clientfd].events = POLLIN;
	
				maxfd = clientfd;
			} 
	
			int i = 0;
			for (i = sockfd+1;i <= maxfd;i ++ ) {
	
				if (fds[i].revents & POLLIN) {
	
					char buffer[128] = {0};
					int count = recv(i, buffer, 128, 0);
					if (count == 0) {
						printf("disconnect\n");
	
						fds[i].fd = -1;
						fds[i].events = 0;
				
						close(i);
						
						continue;
					}
					
					send(i, buffer, count, 0);
					printf("clientfd: %d, count: %d, buffer: %s\n", i, count, buffer);
	
				}
	
			}
	
		}
	return 0;
}

