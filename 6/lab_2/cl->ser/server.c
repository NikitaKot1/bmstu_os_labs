#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

#define SOCK_NAME "socket"

int fd;

void del_socket(void)
{
    if (close(fd) == -1)
    {
        printf("close() failed");
        return;
    }
    if (unlink(SOCK_NAME) == -1)
    {
        printf("unlink() returned -1");
    }
}

void sig_handler(int signum)
{
	printf("SIGTSTP\n");
    del_socket();
    exit(0);
}

int main(void)
{
    struct sockaddr srvr_name;
    char buf[1024];
    int  bytes;

    if ((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket() failed");
        return EXIT_FAILURE;
    }

    srvr_name.sa_family = AF_UNIX;
    strcpy(srvr_name.sa_data, SOCK_NAME);

    if (bind(fd, &srvr_name, strlen(srvr_name.sa_data) + sizeof(srvr_name.sa_family)) == -1)
    {
        printf("bind() failed\n");
        del_socket();
        return EXIT_FAILURE;
    }

    signal(SIGTSTP, sig_handler); //изменение обработчика сигнала

    printf("Waiting for messages.\nCtrl + Z to stop...\n\n");

    while (1)
    {
        bytes = recvfrom(fd, buf, sizeof(buf),  0, NULL, NULL);
        if (bytes < 0)
        {
            del_socket();
            printf("recvfrom() failed");
            return EXIT_FAILURE;
        }
        buf[bytes] = 0;
        printf("Server recieved: %s\n", buf);
    }

    del_socket();
    return EXIT_SUCCESS;
}