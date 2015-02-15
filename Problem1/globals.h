#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#define PROMPTS 1
#define DEBUG 1
#define THREADS 20
#define UNCLASSIFIED 8
#define SECRET 6
#define TOPSECRET 6
#define	TRUE 1
#define CLUSTERSIZE 2
#define SAYWHATSHAPPENING 1

enum securityLevel{
	unclassified,
	secret,
	topsecret,
	UNDEFINED
};

typedef struct threadContainer{
	pthread_t thread;
	enum securityLevel thisLevel;
	int threadNumber;
	struct threadContainer *next;

}NODE;

//Creates a pointer type for a link to the next node.
typedef NODE *Link;

int numberOfProcesses;
Link *unclassifiedPointer ; //A very good place to start
Link *secretPointer;
Link *topsecretPointer;

//WE ARE OFFICIALLY USING SEMAPHORES
sem_t *semaphoreArray[THREADS];
sem_t *semaphoreArray2[THREADS];
Link cluster[2];
Link deadThreads[THREADS];
sem_t holdUntilStarted;
sem_t unlockThread;

//int addEvent(Link *sPtr, int processID, int arrivalTime, int CPUTime);

void bringToBack(Link node, Link *sPtr);

NODE createNode(enum securityLevel thisLevel, int threadNumber);

Link createLink(NODE thisNode);

Link getNextEvent(Link *sPtr);

void printNode(Link node);

void putItIn(Link event, int location);

void revitalize(int *unclassifieds, int *secrets, int *topsecretss);

int processorSimulation(Link *sProcPtr, int timeSlice, int systemTimeIHMs);

void calcMeanAndVariance(double newTime);

void FCFS();

enum securityLevel checkCluster(int x);

enum securityLevel getEnumFromInt(int x);

enum securityLevel pickAQueue(int restraint);

int isEmpty(Link *sPtr);

void printCluster();