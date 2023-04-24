#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>

_Bool sign_flag = false;

void catch_sig(int sig_numb)
{
    signal(sig_numb, catch_sig);
    printf("signal was added\n");
    sign_flag = true;
}


int main(void)
{
    pid_t childpid [2];
    int fd[2];
    pid_t exit_cpid;
    int status;

    signal(SIGINT, catch_sig);
    sleep(3);

    char* mess[2] = {"mekkkkkkkkkk", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"};
    
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

            printf("Child : pid - %d, parent - %d, gid - %d\n", getpid(), getppid(), getpgrp());
            if (sign_flag)
            {
                close(fd[0]);
                write(fd[1], mess[i], sizeof(mess[i]));
            }
            return 0;
        }
        else  printf("Parent:  pid - %d, gid - %d, child - %d\n", getpid(), getpgrp(), childpid[i]);
    }

    for (int i = 0;i < 2; i++)
    {
        if (sign_flag)
        {
            close(fd[1]);
            read(fd[0], (mess[i]), sizeof(mess[i]));
            printf("%s\n", mess[i]);
        }
    
        exit_cpid = wait(&status);
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
}