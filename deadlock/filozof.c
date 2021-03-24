#define PHILOSOPHER
#include "header.h"
#include <time.h>

long philoNumber;
int rightFork;
int leftFork;
int busy;
int freed = -1;

void init()
{
    key = getKey();
    semID = getSemID();
    getSharedMemory();
    srand(getpid());
}

void prepareForks()
{
    rightFork = philoNumber;
    busy = philoNumber;
    leftFork = (philoNumber + 1) % 5;
}

void think()
{
    printf("F%ld is thinking...\n", philoNumber);
    fflush(stdout);
   // sleep(rand() % 3);
}

void eat()
{
    takenFork[rightFork] = busy;
    takenFork[leftFork] = busy;
    printf("F%ld is eating!\n", philoNumber);
    fflush(stdout);
   // sleep(rand() % 3);
    takenFork[rightFork] = freed;
    takenFork[leftFork] = freed;
}

void task()
{
    while (1)
    {
        think();
        waitSem(leftFork);  //czeka na lewy widelec
        waitSem(rightFork); //czeka na prawy widelec
        eat();
        signalSem(rightFork);
        signalSem(leftFork);

    }
}

void sigHandler()
{
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
    signal(SIGINT, sigHandler);
    init();
    philoNumber = convertToLong(argv[1]);
    printf("I am philo nr: %ld\n", philoNumber);
    prepareForks();
    task();
    return 0;
}