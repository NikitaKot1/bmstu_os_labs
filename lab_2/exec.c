#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    const char myapps[2][32] = {"./Stack/app", "./Queue/app"};
    pid_t childpid [2];

    for (int i = 0; i < 2; i++)
    {
        if ((childpid[i] = fork( ))==-1) 
        {
            perror("Can't fork\n");
            exit(1) ;
        } 
        else if (childpid[i] == 0)
        {

            printf("Child: pid - %d, parent - %d, gid - %d\n", getpid(), getppid(), getpgrp());
            if (execl(myapps[i], 0) == -1)
                perror("Can't exec\n");
        }
        else  printf("Parent:  pid - %d, gid - %d, child - %d\n", getpid(), getpgrp(), childpid[i]);
    }

    for (int i = 0; i < 2; i++)
    {
        pid_t exit_cpid;
        int status;
        exit_cpid = waitpid(childpid[i], &status, 0);
        if (exit_cpid == -1)
        {
            printf("Wait error\n");
            exit(1) ;
        }
        printf("Child finished, cpid - %d\n", exit_cpid);
        if (WIFEXITED(status))
            printf("Child exited with code %d\n", WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("Child process terminated by non-intercepted interrupt\nSingal number: %df\n", WTERMSIG(status));
        else if (WIFSTOPPED(status))
            printf("Child process was stopped\nSignal number: =%d", WSTOPSIG(status));
        
    }
    return 0;
}