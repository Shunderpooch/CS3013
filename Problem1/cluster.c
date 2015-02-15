#include "globals.h"

void* runYoself(void *index){
	while (TRUE){
		sem_wait(semaphoreArray[(*((int *)index))]);
		sem_post(&holdUntilStarted);
		useconds_t randomNum = (rand()%1750)*1000; //Get between 0 and 1.75 mil. microseconds
		randomNum += 250000; //add .25 mil. microseconds
		printf("The thread with index %d has started\n", (*((int *)index)));
		usleep(randomNum); //sleep between .25 and 2 seconds
		printf("The thread with index %d has finished\n", (*((int *)index)));
		sem_post(semaphoreArray[(*((int *)index))]);
		useconds_t randomNum2 = (rand()%6000)*1000; // wait to return to the queue
		randomNum2 += 5000000;
		usleep(randomNum2);
		sem_post(semaphoreArray[(*((int *)index))]);
		sem_wait(semaphoreArray2[(*((int *)index))]);
	}
}



//Prints the Node
void printNode(Link node) {
	printf("Thread %d with a security level of %d has thread %d following it.\n", node->threadNumber, node->thisLevel, node->next->threadNumber);
}

int main(){
	srand(time(NULL)); //New random seed
	int i;
	int s;
	int j = 0;
	int k = 0;
	Link unclassifiedHolder[UNCLASSIFIED];
	Link secretHolder[SECRET];
	Link topsecretHolder[TOPSECRET];
	//Create the threads

	for (s = 0; s < THREADS; s++){
		sem_t *tempaphore = malloc(sizeof(sem_t));
		sem_init(tempaphore, 0, 0);
		semaphoreArray[s]= tempaphore;
	}
	for (s = 0; s < THREADS; s++){
		sem_t *tempaphore = malloc(sizeof(sem_t));
		sem_init(tempaphore, 0, 0);
		semaphoreArray2[s]= tempaphore;
	}

	sem_init(&holdUntilStarted, 0, 0);
	sem_init(&unlockThread, 0, 0);

	for (i = 0; i < THREADS; i++){
		NODE tempHolder;
		Link tempLink;
		int err;
		int *newPtr = (int *)malloc(sizeof(int));
		if (i < UNCLASSIFIED){ //Set each element to a classification label
			tempHolder = createNode(unclassified, i);
			*newPtr = i;
			err = pthread_create(&(tempHolder.thread), NULL, &runYoself, (void *)newPtr);
			if (err != 0){
				printf("Cannot create thread: [%s]\n", strerror(err));
			}
			tempLink = createLink(tempHolder);
			unclassifiedHolder[i] = tempLink;
			if (i == (UNCLASSIFIED - 1)){
				unclassifiedHolder[i]->next = createLink(createNode(UNDEFINED, -1));
			}
			if (i > 0) {
				unclassifiedHolder[i - 1]->next = unclassifiedHolder[i];
			}

		}

		else if (i < (UNCLASSIFIED + SECRET)){ //Set others to a secret
			tempHolder = createNode(secret, i);
			*newPtr = i;
			err = pthread_create(&(tempHolder.thread), NULL, &runYoself, (void *)newPtr);
			if (err != 0){
				printf("Cannot create thread: [%s]\n", strerror(err));
			}
			tempLink = createLink(tempHolder);
			secretHolder[j] = tempLink;
			if (j == (SECRET - 1)){
				secretHolder[j]->next = createLink(createNode(UNDEFINED, -1));
			}
			if (j > 0) {
				secretHolder[j - 1]->next = secretHolder[j];
			}
			j++;
		}

		else { //Set even more to topsecret
			tempHolder = createNode(topsecret, i);
			*newPtr = i;
			err = pthread_create(&(tempHolder.thread), NULL, &runYoself, (void *)newPtr);
			if (err != 0){
				printf("Cannot create thread: [%s]\n", strerror(err));
			}
			tempLink = createLink(tempHolder);
			topsecretHolder[k] = tempLink;
			if (k == (TOPSECRET - 1)){
				topsecretHolder[k]->next = createLink(createNode(UNDEFINED, -1));
			}
			if (k > 0) {
				topsecretHolder[k - 1]->next = topsecretHolder[k];
			}
			k++;
		}

	}
	unclassifiedPointer = unclassifiedHolder; //A very good place to start
	secretPointer = secretHolder;
	topsecretPointer = topsecretHolder;

	int x;
	Link *tempUPtr = unclassifiedPointer;
	Link *tempSPtr = secretPointer;
	Link *tempTPtr = topsecretPointer; 
	for (x = 0; x < 8; x++){
		if ( (*tempUPtr)->next != NULL){
			printNode(*tempUPtr);
			tempUPtr++;
		}
	}
	for (x = 0; x < SECRET; x++){
		if ( (*tempSPtr)->next != NULL){
			printNode(*tempSPtr);
			tempSPtr++;
		}
	}
	for (x = 0; x < TOPSECRET; x++){
		if ( (*tempTPtr)->next != NULL){
			printNode(*tempTPtr);
			tempTPtr++;
		}
	}
	cluster[0] = createLink(createNode(UNDEFINED, -1));
	cluster[1] = createLink(createNode(UNDEFINED, -1));
	for(x = 0; x < THREADS; x++){
		deadThreads[x] = createLink(createNode(UNDEFINED, -1));
	}
	FCFS();
	return 1;
}
