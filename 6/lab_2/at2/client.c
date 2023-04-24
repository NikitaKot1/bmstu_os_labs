#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <netinet/in.h>

#define DEFAULT_PORT    8080
#define MAX_CONN        16
#define MAX_EVENTS      32
#define BUF_SIZE        16
#define MAX_LINE        256

static void set_sockaddr(struct sockaddr_in *addr)
{
	bzero((char *)addr, sizeof(struct sockaddr_in));
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = INADDR_ANY;
	addr->sin_port = htons(DEFAULT_PORT);
}

int main()
{
	int n;
	int c;
	int sockfd;
	char buf[MAX_LINE];
	struct sockaddr_in srv_addr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		perror("socket() failed\n");
		exit(1);
	}

	set_sockaddr(&srv_addr);

	if (connect(sockfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
		perror("connect()");
		exit(1);
	}

	char client_buf[2048];
	pid_t pid = getpid();
    sprintf(client_buf, "client: %d\n\0", pid);
	printf(client_buf);

	n = write(sockfd, client_buf, strlen(client_buf) + 1);

    if (n == -1)
        perror("cant write\n");

    sleep(1);

	read(sockfd, client_buf, strlen(client_buf));
	printf("echo: %s\n", client_buf);
	close(sockfd);
}
