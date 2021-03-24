#define PHILOSOPHER
#include "header.h"
#include <time.h>

long philoNumber;
int rightFork;
int leftFork;
int busy;
int freed = -1;
int eatingCounter = 0;

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
    //sleep(rand() % 3);
}

void eat()
{
    eatingCounter++;
    takenFork[rightFork] = busy;
    takenFork[leftFork] = busy;
    printf("F%ld is eating %d time!\n", philoNumber, eatingCounter);
    fflush(stdout);
    //sleep(rand() % 3);
    takenFork[rightFork] = freed;
    takenFork[leftFork] = freed;
}

void task()
{
    while (1)
    {
        think();
        waitSem(leftFork, rightFork);
        eat();
        signalSem(leftFork, rightFork);
    }
}

void sigHandler(int sig){
    printf("Philosophrer %ld has eaten %d times\n", philoNumber,eatingCounter);
    fflush(stdout);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
    signal(SIGINT, sigHandler);
    init();
    philoNumber = convertToLong(argv[1]);
    prepareForks();
    task();
    return 0;
}