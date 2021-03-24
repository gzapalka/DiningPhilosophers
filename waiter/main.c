
#include "header.h"
#include <wait.h>


void createPhilo()
{

    char philoNumber[sizeof(int)];
    for (int i = 0; i < 5; i++)
    {
        sprintf(philoNumber, "%d", i);
        int id = fork();
        switch (id)
        {
        case -1:
            perror("Blad tworzenia filozofow");
            exit(EXIT_FAILURE);
        case 0:
            execl("./filozof", "filozof", philoNumber, NULL);
            perror("Blad execl filozofa");
            exit(EXIT_FAILURE);
        }
    }

    wait(0);
}

void init()
{
    key = getKey();
    semID = getSemID();
    initSem();
    getSharedMemory();
    initSharedMemory();
}

void sigHandler(int sig)
{
    if (-1 == semctl(semID, N, IPC_RMID, NULL))
    {
        perror("Blad usuniecia semaforow");
    }
    if (-1 == shmctl(shmID, IPC_RMID, NULL))
    {
        perror("Blad usuniecia pamieci dzielonej");
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
    signal(SIGINT, sigHandler);
    init();
    createPhilo();

    return 0;
}