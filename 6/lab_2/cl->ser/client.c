#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SOCK_NAME "socket"
#define BUF_SIZE 256

void understand()
{
    switch (errno) {
        case EACCES:
            printf("EACCES\n");
            break;
        case EAFNOSUPPORT:
            printf("EAFNOSUPPORT\n");
            break;
        case EINVAL:
            printf("EINVAL\n");
            break;
        case EMFILE:
            printf("EMFILE\n");
            break;
        case ENFILE:
            printf("ENFILE\n");
            break;
        case ENOBUFS:
            printf("ENOBUFS\n");
            break;
        case EPROTONOSUPPORT:
            printf("EPROTONOSUPPORT\n");
            break;
        case ENOMEM:
            printf("ENOMEM\n");
            break;
        default:
            printf("def\n");
            break;
    }
}

int main(void)
{
	int fd = socket(AF_UNIX, SOCK_DGRAM, 0);

    if (fd == -1)
    {
        printf("socket failed");
        return EXIT_FAILURE;
    }

    struct sockaddr addr;
    addr.sa_family = AF_UNIX;
    strcpy(addr.sa_data, SOCK_NAME);

	char buf[BUF_SIZE];
    sprintf(buf, "pid %d", getpid());

	
    if (sendto(fd, buf, strlen(buf), 0, &addr, strlen(addr.sa_data) + sizeof(addr.sa_family)) == -1)
    {
        perror("sendto() failed:\n");
        close(fd);
        //understand();
        return EXIT_FAILURE;
    }

    printf("Client sent: %s\n", buf);



    close(fd);
    return EXIT_SUCCESS;
}