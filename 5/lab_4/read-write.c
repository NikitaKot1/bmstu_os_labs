#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define WRITERS_N 3
#define READERS_N 5

#define P -1
#define V 1

#define NOW_READ 0
#define NOW_WRITE 1
#define ACTIVE_READ 2
#define WAIT_WRITE 3

const int PERM =  S_IRWXU | S_IRWXG | S_IRWXO;

struct sembuf start_read[] = { {NOW_READ, V, 0}, {NOW_WRITE, 0, 0}, {WAIT_WRITE, 0, 0}, {ACTIVE_READ, V, 0}, {NOW_READ, P, 0} };
struct sembuf stop_read[] = { {ACTIVE_READ, P, 0} };
struct sembuf start_write[] = { {WAIT_WRITE, V, 0}, {ACTIVE_READ, 0, 0}, {NOW_WRITE, 0, 0}, {NOW_WRITE, V, 0}, {WAIT_WRITE, P, 0} };
struct sembuf stop_write[] = { {NOW_WRITE, P, 0} };

void reader(int semid, int* shm) 
{
    while (1)
    {
        semop(semid, start_read, 5);
        srand(getpid());  
        int sl = 500000 + rand() % 2000000; 
		printf("\e[1;33mReader %d == %d\e[0m\n", getpid(), *shm);
        semop(semid, stop_read, 1);   
        usleep(sl);
    }
}

void writer(int semid, int* shm) 
{
    while (1)
    {
        semop(semid, start_write, 5);
        srand(getpid());   
        int sl = 500000 + rand() % 2000000;
        (*shm)++;
        printf("\e[1;35mWriter %d == %d\e[0m\n", getpid(), *shm);
        semop(semid, stop_write, 1);    
        usleep(sl);
    }
}

int main() 
{
    int shm_id;
    if ((shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | PERM)) == -1) 
	{
		perror("Error create shared memory\n");
		exit( 1 );
	}

    struct shmid_ds shm_desk;

    int *shm_buf = shmat(shm_id, NULL, 0); 
    if (shm_buf == (void*) -1) 
    {
        perror("Error attach shared memory\n");
        exit( 1 );
    }

    (*shm_buf) = 0;

    int sem_id;
    if ((sem_id = semget(IPC_PRIVATE, 4, IPC_CREAT | PERM)) == -1) 
	{
		perror("Error create semaphore\n");
		exit( 1 );
	}

    int s1 = semctl(sem_id, 0, SETVAL, 0);
	int s2 = semctl(sem_id, 1, SETVAL, 0);
	int s3 = semctl(sem_id, 2, SETVAL, 0);
    int s4 = semctl(sem_id, 3, SETVAL, 0);

	if (s1 == -1 || s2 == -1 || s3 == -1 || s4 == -1)
	{
		perror("semctl error\n");
		exit(1);
	}

	pid_t pid = -1;

	for (int i = 0; i < WRITERS_N && pid != 0; i++) 
    {
        pid = fork();
        if (pid == -1) 
        {
            perror("Fork error\n"); 
            exit(1);
        }

        if (pid == 0) 
        {
            writer(sem_id, shm_buf);
            return 0;
        }
	}

    for (int i = 0; i < READERS_N && pid != 0; i++) 
    {
        pid = fork();
        if (pid == -1) 
        {
            perror("Fork error\n"); 
            exit(1);
        }

        if (pid == 0) 
        {
            reader(sem_id, shm_buf);
            return 0;
        }
	}

    while (1)
    {
        semop(sem_id, start_read, 5);
        srand(getpid());  
        int sl = 500000 + rand() % 2000000; 
		printf("\e[1;33mMain Reader %d == %d\e[0m\n", getpid(), *shm_buf);
        semop(sem_id, stop_read, 1);   
        usleep(sl);
    }

    for (int i = 0; i < WRITERS_N + READERS_N; ++i) {        
        wait(NULL);
        
        if (shmdt(shm_buf) == -1) 
        {
            perror("Error detach shared memory\n");
            exit(1);
        }
    }

    if (shmctl(sem_id, IPC_RMID, &shm_desk) == -1)
        perror("main: shmctl; ");

    if (semctl(sem_id, 0, IPC_RMID) == -1)
    {
        perror("semctl with command IPC_RMID error");
        exit(1);
    }

    return 0;
}