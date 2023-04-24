#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    pid_t childpid [2];
    for (int i = 0; i < 2; i++)
    {
        if ((childpid[i] = fork( ))==-1) 
        {
            perror("Can't fork");
            exit(1) ;
        } 
        else if (childpid[i] == 0)
        {
            printf("Child: pid - %d, parent - %d, gid - %d\n", getpid(), getppid(), getpgrp());
            sleep(2);
            printf("Child : pid - %d, parent - %d, gid - %d\n", getpid(), getppid(), getpgrp());
            return 0;
        }
        else  printf("Parent:  pid - %d, gid - %d, child - %d\n", getpid(), getpgrp(), childpid[i]);
    }

    for (int i = 0;i < 2; i++)
    {
        pid_t exit_cpid;
        int status;
        exit_cpid = wait(&status);
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