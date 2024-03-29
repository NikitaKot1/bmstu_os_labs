#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/epoll.h>

#define DEFAULT_PORT    9877
#define MAX_CONN        16
#define MAX_EVENTS      32
#define BUF_SIZE        16
#define MAX_LINE        256

int fd;

static void set_sockaddr(struct sockaddr_in *addr)
{
	bzero((char *)addr, sizeof(struct sockaddr_in));
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = INADDR_ANY;
	addr->sin_port = htons(DEFAULT_PORT);
}

static void epoll_ctl_add(int epfd, int fd, uint32_t events)
{
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		perror("epoll_ctl()\n");
		exit(1);
	}
}

static int setnonblocking(int sockfd)
{
	if (fcntl(sockfd, F_SETFD, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK) ==-1) {
		return -1;
	}
	return 0;
}

int main(void)
{
	int i;
	int n;
	int epfd;
	int nfds;
	int listen_sock;
	int conn_sock;
	int socklen;
	char buf[BUF_SIZE];
	struct sockaddr_in srv_addr;
	struct sockaddr_in cli_addr;
	struct epoll_event events[MAX_EVENTS];

	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	//listen_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (listen_sock == -1)
	{
		perror("listen sock == -1!\n");
		exit(1);
	}

	set_sockaddr(&srv_addr);
	if (bind(listen_sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) == -1)
	{
		perror("error!");
		exit(1);
	}

	if (setnonblocking(listen_sock) == -1)
		perror("fcntl!\n");
	listen(listen_sock, MAX_CONN);

	epfd = epoll_create(1);
	epoll_ctl_add(epfd, listen_sock, EPOLLIN | EPOLLOUT | EPOLLET);

	socklen = sizeof(cli_addr);
	for (;;) {
		nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
		for (i = 0; i < nfds; i++) 
		{
			if (events[i].data.fd == listen_sock) 
			{
				/* handle new connection */
				if (conn_sock = accept(listen_sock, (struct sockaddr *)&cli_addr, &socklen) == -1)
					perror("Cent accept!");
				inet_ntop(AF_INET, (char *)&(cli_addr.sin_addr), buf, sizeof(cli_addr));
				printf("Client: %s:%d\n", buf, ntohs(cli_addr.sin_port));

				if (setnonblocking(conn_sock) == -1)
					perror("fcntl!\n");
				epoll_ctl_add(epfd, conn_sock, EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
			} else if (events[i].events & EPOLLIN) 
					{
						/* handle EPOLLIN event */
						bzero(buf, sizeof(buf));
						n = read(events[i].data.fd, buf, sizeof(buf));
						if (n > 0 ) 
						{
							printf("Client pid: %s\n", buf);
							n = write(events[i].data.fd, buf, strlen(buf));
							if (n < 0)
								perror("Cant write!");
						}
						
					}
			/* check if the connection is closing */
			if (events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
				printf("Connection closed\n");
				if (epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL) == -1)
					perror("cant epoll delete!");
				close(events[i].data.fd);
			}
		}
	}
}