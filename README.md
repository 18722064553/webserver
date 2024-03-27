![2e19bb5ec83f4d6328fac2d310c7abd](https://github.com/18722064553/webserver/assets/80340040/381c0017-78d9-4152-9f21-a517a04926f8)- multi-io.c：用于单服务器多客户端TCP通信
- multi-thread-io.c：用线程函数实现单服务器多客户端TCP通信
  -  ``gcc -o multi-thread-io multi-thread-io.c -lpthread``
- selcet.c：在一个线程内实现单服务器多客户端的TCP通信
  - ``select(maxfd, rset, wset, eset, timeout)``
	  - maxfd：最大的fd（fd是依次增加的int值。其中0是stdin，1是stdout，2是stderror）
	  - rset：可读集合、wset：可写集合、eset：出错集合
	  - timeout：超时（多次时间轮询一次）
  - select处理的最大int长度是1024，是在内核里确定的，如果io更大则会提示数组越界
- poll.c：是select在参数上的一个优化，减少了 一些不必要参数的复制，他们底层实现是一样，都是基于select设计的
- epoll.c：解决了大量io的问题，在epoll加入linux内核后使得linux可以用在服务端，包括三个函数：
  - epoll_create()
  - epoll_ctl()
  - epoll_wait() 
- reactor.c：封装了epoll，通过事件驱动的方式管理并发连接!
- webserver.c：可以使得一个网页向服务器发送连接请求
- millions_connections.c：通过mul_port_client_epoll.c向服务端不断发送连接请求以建立连接，最终实现百万级连接



