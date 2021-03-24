#ifndef HEADER_FILE
#define HEADER_FILE

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <signal.h>
#define N 5

key_t key;
int semID;
int shmID;
int *takenFork;

long convertToLong(char *arg)
{
    char *buff;

    errno = 0;
    long value = strtol(arg, &buff, 10);
    ;
    if ((errno == ERANGE && (value == LONG_MAX || value == LONG_MIN)) || (errno != 0 && value == 0))
    {
        perror("Blad strtol");
        exit(EXIT_FAILURE);
    }

    if (buff == arg)
    {
        printf("Argument nie jest liczba");
        exit(EXIT_FAILURE);
    }

    return value;
}

key_t getKey()
{
    key_t key = ftok(".", 'G');
    if (key == -1)
    {
        perror("Blad ftok");
        exit(EXIT_FAILURE);
    }

    return key;
}

int getSemID()
{
#ifdef PHILOSOPHER
    int semID = semget(key, N, IPC_CREAT | 0600);
#else
    int semID = semget(key, N, IPC_CREAT | IPC_EXCL | 0600);
#endif

    if (semID == -1)
    {
        perror("Blad semID");
        exit(EXIT_FAILURE);
    }
    return semID;
}

void initSem()
{
    for (int i = 0; i < N; i++)
    {
        if (-1 == semctl(semID, i, SETVAL, 1))
        {
            perror("Blad inicjowania wartosci semaforow widelcy");
            exit(EXIT_FAILURE);
        }
    }
}

void getSharedMemory()
{
#ifdef PHILOSOPHER
    shmID = shmget(key, 5 * sizeof(int), IPC_CREAT | 0600);
#else
    shmID = shmget(key, 5 * sizeof(int), IPC_CREAT | IPC_EXCL | 0600); //pewnie trzeba bedzie usunac o ipc_excl bo po co tu ona xd
#endif
    if (shmID == -1)
    {
        perror("Blad pozyskania shmID");
        exit(EXIT_FAILURE);
    }

    takenFork = (int *)shmat(shmID, NULL, 0);
}

void initSharedMemory()
{
    for (int i = 0; i < 5; i++)
    {
        takenFork[i] = -1;
    }
}

void waitSem(int number1, int number2)
{
    struct sembuf semOperation[2];
    semOperation[0].sem_num = number1;
    semOperation[0].sem_op = -1;
    semOperation[0].sem_flg = 0;

    semOperation[1].sem_num = number2;
    semOperation[1].sem_op = -1;
    semOperation[1].sem_flg = 0;

    while (semop(semID, semOperation, 2) == -1)
    {
        if (errno == EINTR)
        {
            continue;
        }
        else
        {
            perror("Nie udalo sie ustawic semafora");
            exit(EXIT_FAILURE);
            break;
        }
    }
}

void signalSem(int number1, int number2)
{
    struct sembuf semOperation[2];
    semOperation[0].sem_num = number1;
    semOperation[0].sem_op = +1;
    semOperation[0].sem_flg = 0;

    semOperation[1].sem_num = number2;
    semOperation[1].sem_op = +1;
    semOperation[1].sem_flg = 0;
    while (semop(semID, semOperation, 2) == -1)
    {
        if (errno == EINTR)
        {
            continue;
        }
        else
        {
            perror("Nie udalo sie ustawic semafora");
            exit(EXIT_FAILURE);
            break;
        }
    }
}

#endif