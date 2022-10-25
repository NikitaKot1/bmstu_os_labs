#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    char* mess[2] = {"mekkkkkkkkkk", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"};
    pid_t childpid [2];

    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("Can't pipe\n");
        return 1;
    }

    for (int i = 0; i < 2; i++)
    {
        if ((childpid[i] = fork( ))==-1) 
        {
            perror("Can't fork\n");
            exit(1) ;
        } 
        else if (childpid[i] == 0)
        {

            printf("Child %d: pid - %d, parent - %d, gid - %d\n", i + 1, getpid(), getppid(), getpgrp());
            close(fd[0]);
            write(fd[1], mess[i], sizeof(mess[i]));
            return 0;
        }
        else  printf("Parent:  pid - %d, gid - %d, child - %d\n", getpid(), getpgrp(), childpid[i]);
    }

    for (int i = 0;i < 2; i++)
    {
        close(fd[1]);
        read(fd[0], mess[i], sizeof(mess[i]));
        printf("%s\n", mess[i]);
        pid_t exit_cpid;
        int status;
        exit_cpid = waitpid(childpid[i], &status, 0);
        if (exit_cpid == -1)
            printf("Wait any child process");
        else if (exit_cpid == 0)
            printf("Wait any child process with gip = %d", getpgrp());
        else 
        {
            printf("Child finished, cpid - %d\n", exit_cpid);
            if (WIFEXITED(status))
                printf("Child exited with code %d\n", WEXITSTATUS(status));
            else if (WIFSIGNALED(status))
                printf("Child process terminated by non-intercepted interrupt\nSingal number: %df\n", WTERMSIG(status));
            else if (WIFSTOPPED(status))
                printf("Child process was stopped\nSignal number: =%d", WSTOPSIG(status));
        }
    }

    return 0;
}