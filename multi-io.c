#include <sys/socket.h>
#include <error.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_LENGTH 1024

int main(){
	// 创建socket：int socket(int domain, int type, int protocol)
	// domain参数告诉系统使用哪个底层协议族:
	// 对于TCP/IP协议族而言，AF_INET用于IPv4，AF_INET6用于IPv6
	// type参数指定服务类型。服务类型主要有SOCK_STREAM服务（流服务）和SOCK_UGRAM（数据报）服务
	// protocol参数一般设置为0，表示使用默认协议
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	// sockaddr_in是TCP/IP协议族专用socket地址结构体，用于IPv4
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(2048);
	
	// 命名socket：int bind(int sockfd, const struct sockaddr* my_addr, socklen_t addrlen)
	// 在服务器程序中，我们要给socket命名这样客户端才能知道如何连接它，客户端则不需要命名socket
	// bind将my_addr所指的socket地址分配给未命名的sockfd文件描述符
	// addrlen参数指出该socket地址的长度
	int name_fd = bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr));
	if(name_fd == -1){
		perror("bind error");
		return -1;
	}
	
	// 监听socket：int listen(int sockfd, int backlog)
	// backlog参数提示内核监听队列的最大长度，如果长度超过backlog，服务器将不受理新的客户连接
	name_fd = listen(sockfd, 10);
	
	// 接受连接：int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
	// 从listen监听队列中接受一个连接
	// sockfd参数是执行过listen系统调用的监听socket
	// addr参数用来获取被接受连接的远端socket地址，该socket地址的长度由addrlen参数指出
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);
	int clientfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
	
	// TCP数据读写：
	// ssize_t recv(int sockfd, void *buf, size_t len, int flags)
	// recv读取sockfd上的数据，buf和len参数分别指定读缓冲区的位置和大小，flags参数通常设置为0
	// ssize_t send(int sockfd, const void *buf, size_t len, int flags)
	// send往sockfd上写入数据，buf和len参数分别指定写缓冲区的位置和大小
	while(1){  // while循环可以多次接收客户端发送的数据
		char buffer[BUFFER_LENGTH] = {0};
		int ret = recv(clientfd, buffer, BUFFER_LENGTH, 0);
		// 服务器解析收到的内容然后才能确定发送给谁
		if(ret == 0){
			close(clientfd);
			break;
		}
		printf("%s\n",buffer);
		send(clientfd, buffer, ret, 0);
	}
	return 0;
}

