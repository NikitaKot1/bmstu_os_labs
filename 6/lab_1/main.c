#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define BUF_SIZE 32
int main(int argc, char ** argv)
{
    int sockets[2];
    char buf[3][BUF_SIZE] = {"a", "b", "c"};
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) 
    {
        perror("socketpair() failed");
        return EXIT_FAILURE;
    }
    pid_t childpid [3];
    for (int i = 0; i < 3; i++)
    {
        if ((childpid[i] = fork( ))==-1) 
        {
            perror("Can't fork");
            exit(1) ;
        } 
        if (childpid[i] == 0) 
        {
            //close(sockets[1]);
            printf("Child%d write: %s\n", i + 1, buf[i]);
            write(sockets[0], buf[i], sizeof(buf[i]));
            buf[i][0] = '\0';
            sleep(1);
            read(sockets[0], buf[i], sizeof(buf[i]));
            printf("Child%d  read: %s\n", i + 1, buf[i]);
            //close(sockets[0]);
            return 0;
        } 
        else 
        {
            buf[i][0] = '\0';
            sleep(1);
            read(sockets[1], buf[i], sizeof(buf[i]));
            printf("Parent  read: %s\n", buf[i]);
            printf("Parent write: %s\n", buf[i]);
            write(sockets[1], buf[i], sizeof(buf[i]));
        }
    }
   // sleep(2);
    close(sockets[0]);
    close(sockets[1]);
    return 0;
}