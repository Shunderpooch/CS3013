#include "globals.h"

//Checks to see if thread in cluster[x] is empty, done, or still running. Returns what type is running.
enum securityLevel checkCluster(int x) {
	if (x >= CLUSTERSIZE || x < 0) {
		printf("You are requesting the status of an area of the cluster that does not exist\n");
		return UNDEFINED;
	}
	else if (cluster[x]->thisLevel == UNDEFINED) { //OH LOOK! FILLER!
		return UNDEFINED;
	}
	if (sem_trywait(semaphoreArray[(cluster[x]->threadNumber)]) == 0) { //The thread that was running has ended.
		deadThreads[cluster[x]->threadNumber] = cluster[x];
		printf("%d process has finished.\n", cluster[x]->threadNumber);
		cluster[x] = createLink(createNode(UNDEFINED, -1));
		return UNDEFINED;
	}
	else {
		return cluster[x]->thisLevel;
	}
}

int getMode() {
	if (checkCluster(0) == UNDEFINED && checkCluster(1) == UNDEFINED) {
		return 0;
	}
	else if (checkCluster(0) == unclassified || checkCluster(1) == unclassified) {
		return 1;
	}
	else{
		return 2;
	}
}

void putItIn(Link event, int location) {	
	printf("%d thread is being turned on.\n", event->threadNumber);
	sem_post(semaphoreArray[event->threadNumber]);
	cluster[location] = event;
	sem_wait(&holdUntilStarted);
	printCluster();
}

void printCluster() {
	printf("\nElement1 in cluster is %d with security level %d\n", cluster[0]->threadNumber, cluster[0]->thisLevel);
	printf("Element2 in cluster is %d with security level %d\n\n", cluster[1]->threadNumber, cluster[1]->thisLevel);
}


void revitalize(int *unclassifieds, int *secrets, int *topsecrets) {
	int x;
	for(x = 0; x < THREADS; x++) {
		if(deadThreads[x]->threadNumber == x) {
			if (sem_trywait(semaphoreArray[x]) == 0) {
				sem_post(semaphoreArray2[x]);
				printf("Released a thread  %d from death.\n", deadThreads[x]->threadNumber);
				if (deadThreads[x]->thisLevel == unclassified) {
					bringToBack(deadThreads[x], unclassifiedPointer);
					(*unclassifieds)++;
				}

				else if (deadThreads[x]->thisLevel == secret) {
					bringToBack(deadThreads[x], secretPointer);
					(*secrets)++;
				}

				else if (deadThreads[x]->thisLevel == topsecret) {
					bringToBack(deadThreads[x], topsecretPointer);
					(*topsecrets)++;
				}
				deadThreads[x] = createLink(createNode(UNDEFINED, -1));
			}
		
			else {
				continue;
			}
		}
	}
}


enum securityLevel getEnumFromInt(int x) {
	switch (x) {
		case 0:
			return unclassified;
		case 1:
			return secret;
		case 2:
			return topsecret;
	}
	return UNDEFINED;
}

enum securityLevel pickAQueue(int restraint) {
	int choice;
	if (restraint) {
		choice = rand() % 2;
		choice++;
		return (getEnumFromInt(choice));
	}
	else {
		choice = rand() % 3;
		return (getEnumFromInt(choice));
	}
}

//Run the first come, first serve queue
void FCFS() {
	int counter = 0;
	int unclassifieds = UNCLASSIFIED;
	int secrets = SECRET;
	int topsecrets = TOPSECRET;
	enum securityLevel chosenLevel;
	Link element1;
	Link element2;
	Link holderElement;
	int thingsToPutIn = -1;
	int insertedFromTopSecret = 0; // for checking how many top secrt processes have been inserted under 
	//the condition of there being 3 or more top secret processes in the queue
	//Nothing's being used when we start
	//Runs while the list is not empty
	while (TRUE) {
		revitalize(&unclassifieds, &secrets, &topsecrets);
		printf("%d %d %d\n", unclassifieds, secrets, topsecrets);
		while(checkCluster(0) != UNDEFINED && checkCluster(1) != UNDEFINED)	{
			 //busywaiting because all slots in cluster are full
		}
		//First check if we need to put top secrets away
		if (insertedFromTopSecret == 1) { //Check if we still have to insert one more top secret from the greater than three required.
			if (checkCluster(0) == UNDEFINED) {
				element1 = getNextEvent(topsecretPointer);
				if (SAYWHATSHAPPENING){
					printf("We waited to insert a priority top secret process. Now we are putting %d in slot zero.\n", element1->threadNumber);
				}
				putItIn(element1, 0);
				element1 = NULL;
				topsecrets--;
				insertedFromTopSecret = 0;
				continue;
			}
			else if (checkCluster(1) == UNDEFINED) {
				element1 = getNextEvent(topsecretPointer);
				if (SAYWHATSHAPPENING){
					printf("We waited to insert a priority top secret process. Now we are putting %d in slot one.\n", element1->threadNumber);
				}
				putItIn(element1, 1);
				element1 = NULL;
				topsecrets--;
				insertedFromTopSecret = 0;
				continue;
			}
		}
		else if(topsecrets > 2) { //We MUST prioritize top secrets if there are more than two.
			// Completely empty cluster 
			printf("Entering top secret prioritization\n");
			static int shouldPrint = 1;
			while(checkCluster(0) == unclassified || checkCluster(1) == unclassified) {
				/* there are 1 or 2 unclassified processes running in the cluster, we have to wait for it to empty.*/
				if (DEBUG && shouldPrint) {
					printf("Waiting on unclassified process to finish to insert topsecrets\n");
					shouldPrint = 0;
				}
			}
			shouldPrint = 1;
			//There is 1 classified process running, and we can throw in a topsecret immediately
			if (checkCluster(0) == UNDEFINED && (checkCluster(1) == secret || checkCluster(1) == topsecret)) {
				element1 = getNextEvent(topsecretPointer);
				if (SAYWHATSHAPPENING){
					printf("A top secret process is prioritized. Now we are putting %d in slot zero.\n", element1->threadNumber);
				}
				putItIn(element1, 0);
				element1 = NULL;
				topsecrets--;
				insertedFromTopSecret = 1;
				continue;
			}
			else if ((checkCluster(0) == secret || checkCluster(0) == topsecret) && checkCluster(1) == UNDEFINED) {
				element1 = getNextEvent(topsecretPointer);
				if (SAYWHATSHAPPENING){
					printf("A top secret process is prioritized. Now we are putting %d in slot one.\n", element1->threadNumber);
				}
				putItIn(element1, 1);
				element1 = NULL;
				topsecrets--;
				insertedFromTopSecret = 1;
				continue;
			}
			if (checkCluster(0) == UNDEFINED && checkCluster(1) == UNDEFINED) {
				element1 = getNextEvent(topsecretPointer);
				putItIn(element1, 0);
				element2 = getNextEvent(topsecretPointer);
				if (SAYWHATSHAPPENING){
					printf("Two top secret process is prioritized. Now we are putting %d and %d in.\n", element1->threadNumber, element2->threadNumber);
				}
				putItIn(element2, 1);
				element1 = NULL;
				element2 = NULL;
				topsecrets = topsecrets - 2;
				if(DEBUG) {
					printf("There are now %d top secret processes in the queue.\n", topsecrets);  
					printCluster();
				}
				continue;
			}
		}
		else if(thingsToPutIn) {
			if(element2->thisLevel == unclassified) {
				if(getMode() == 1) {
					if(checkCluster(0) == UNDEFINED) {
						if (SAYWHATSHAPPENING){
							printf("A process is waiting to enter, after being selected by 2. Now we are putting %d in slot zero.\n", element2->threadNumber);
						}
						putItIn(holderElement, 0);
						holderElement = NULL;
						thingsToPutIn = 0;
						continue;
					}
					else{
						if (SAYWHATSHAPPENING){
							printf("A process is waiting to enter, after being selected by 2. Now we are putting %d in slot one.\n", element2->threadNumber);
						}
						putItIn(holderElement, 1);
						holderElement = NULL;
						thingsToPutIn = 0;
						continue;
					}
				}
			}
			else {
				if(getMode() == 2) {
					if(checkCluster(0) == UNDEFINED) {
						if (SAYWHATSHAPPENING){
							printf("A process is waiting to enter, after being selected by 2. Now we are putting %d in slot zero.\n", element2->threadNumber);
						}
						putItIn(holderElement, 0);
						holderElement = NULL;
						thingsToPutIn = 0;
						continue;
					}
					else{
						if (SAYWHATSHAPPENING){
							printf("A process is waiting to enter, after being selected by 2. Now we are putting %d in slot one.\n", element2->threadNumber);
						}
						putItIn(holderElement, 1);
						holderElement = NULL;
						thingsToPutIn = 0;
						continue;
					}
				}
			}
			if(getMode() == 0) {
				if (SAYWHATSHAPPENING){
					printf("A process is waiting to enter, after being selected by 2. Now we are putting %d in slot zero.\n", element2->threadNumber);
				}
				putItIn(holderElement, 0);
				holderElement = NULL;
				thingsToPutIn = 0;
				continue;
			}
		}
		else{
			int success = 0;
			if (counter % 2 == 0) { //Pick two elements
				chosenLevel = pickAQueue(0);
				switch(chosenLevel) {
					case unclassified:
						if (unclassifieds > 1) {
							element1 = getNextEvent(unclassifiedPointer);
							element2 = getNextEvent(unclassifiedPointer);
							unclassifieds = unclassifieds - 2;
							success = 1;
						}
						break;
					case secret:
						if (secrets == 1 && !(isEmpty(topsecretPointer))) {
							element1 = getNextEvent(secretPointer);
							element2 = getNextEvent(topsecretPointer);
							secrets--;
							topsecrets--;
							success = 1;
						}
						else if (secrets > 1) {
							element1 = getNextEvent(secretPointer);
							element2 = getNextEvent(secretPointer);
							secrets = secrets - 2;
							success = 1;
						}
						break;
					case topsecret:
						if (topsecrets == 1 && !isEmpty(secretPointer)) {
							element1 = getNextEvent(secretPointer);
							element2 = getNextEvent(topsecretPointer);
							secrets--;
							topsecrets--;
							success = 1;
						}
						else if (topsecrets > 1) {
							element1 = getNextEvent(topsecretPointer);
							element2 = getNextEvent(topsecretPointer);
							topsecrets = topsecrets - 2;
							success = 1;
						}
						break;
					default:
						break;
				}
				if (success) {
					//printf("Here\n");
					static int shouldPrintAgain = 1;
					if (element1->thisLevel == secret || element1->thisLevel == topsecret) {
						while(getMode() == 1) {
						/* there are 1 or 2 unclassified processes running in the cluster, we have to wait for it to empty.*/
							if (DEBUG && shouldPrintAgain) {
								printf("Waiting on unclassified process to finish\n");
								shouldPrintAgain = 0;
							}
						}
						if (getMode() == 2) { //This is the mode we actually can work with ONE PROCESS WITH
							if (checkCluster(0) == UNDEFINED) {
								if (SAYWHATSHAPPENING){
									printf("Two are selected, and the first %d is being put in slot zero.\n", element1->threadNumber);
								}
								putItIn(element1, 0);
								element1 = NULL;
								tempEl
								thingsToPutIn = 1;
								continue;
							}
							else{
								if (SAYWHATSHAPPENING){
									printf("Two are selected, and the first %d is being put in slot one.\n", element1->threadNumber);
								}
								putItIn(element1, 1);
								element1 = NULL;
								thingsToPutIn = 1;
								continue;
							}
						}
					}
					else { 
						while(getMode() == 2) {
							/* there are 1 or 2 classified processes running in the cluster, we have to wait for it to empty.*/
							if (DEBUG && shouldPrintAgain) {
								printf("Waiting on classified process to finish\n");
								shouldPrintAgain = 0;
							}
						}
						if (getMode() == 1) { //This is the mode we actually can work with ONE PROCESS WITH
							if (checkCluster(0) == UNDEFINED) {
								if (SAYWHATSHAPPENING){
									printf("Two are selected, and the first %d is being put in slot zero.\n", element1->threadNumber);
								}
								putItIn(element1, 0);
								element1 = NULL;
								thingsToPutIn = 1;
								continue;
							}
							else{
								if (SAYWHATSHAPPENING){
									printf("Two are selected, and the first %d is being put in slot one.\n", element1->threadNumber);
								}
								putItIn(element1, 1);
								element1 = NULL;
								thingsToPutIn = 1;
								continue;
							}
						}
					}
					shouldPrintAgain = 1;
					if (getMode() == 0) {
						if (SAYWHATSHAPPENING){
							printf("Two are selected, and both %d and %d are being put in.\n", element1->threadNumber, element2->threadNumber);
						}
						putItIn(element1, 0);
						element1 = NULL;
						putItIn(element2, 1);
						element2 = NULL;
						continue;
					}
				}

			}
			 if (counter % 2 == 1 || success == 0) { //Pick one element
				chosenLevel = pickAQueue(0);
				switch(chosenLevel) {
					case unclassified:
						if (!isEmpty(unclassifiedPointer)) {
							element1 = getNextEvent(unclassifiedPointer);
							unclassifieds--;
							success = 1;
						}
						break;
					case secret:
						if (!isEmpty(secretPointer)) {
							element1 = getNextEvent(secretPointer);
							secrets--;
							success = 1;
						}
						break;
					case topsecret:
						if (!isEmpty(topsecretPointer)) {
							element1 = getNextEvent(secretPointer);
							topsecrets--;
							success = 1;
						}
						break;
					default:
						break;
				}
				if (success) {
					//printf("Here\n");
					static int shouldPrintOnceMore = 1;
					
					if (element1->thisLevel == unclassified) {
						while(getMode() == 2) {
							if (DEBUG && shouldPrintOnceMore) {
								printf("Waiting on classified process to finish\n");
								shouldPrintOnceMore = 0;
							}
						}
						shouldPrintOnceMore = 1;
						if(getMode() == 1) {
							if(checkCluster(0) == UNDEFINED) {
								if (SAYWHATSHAPPENING){
									printf("One is selected, the cluster is not empty and %d is being put in slot zero.\n", element1->threadNumber);
								}
								putItIn(element1, 0);
								element1 = NULL;
								continue;
							}
							else{
								if (SAYWHATSHAPPENING){
									printf("One is selected, the cluster is not empty and %d is being put in slot one.\n", element1->threadNumber);
								}
								putItIn(element1, 1);
								element1 = NULL;
								continue;
							}
						}
					}
					else{
						while(getMode() == 1) {
							if (DEBUG && shouldPrintOnceMore) {
								printf("Waiting on unclassified process to finish\n");
								shouldPrintOnceMore = 0;
							}
						}
						shouldPrintOnceMore = 1;
						if(getMode() == 2) {
							if(checkCluster(0) == UNDEFINED) {
								if (SAYWHATSHAPPENING){
									printf("One is selected, the cluster is not empty and %d is being put in slot zero.\n", element1->threadNumber);
								}
								putItIn(element1, 0);
								element1 = NULL;
								continue;
							}
							else{
								if (SAYWHATSHAPPENING){
									printf("One is selected, the cluster is not empty and %d is being put in slot one.\n", element1->threadNumber);
								}
								putItIn(element1, 1);
								element1 = NULL;
								continue;
							}
						}
					}
					if(getMode() == 0) {
						if (SAYWHATSHAPPENING){
							printf("One is selected, and %d is being put in slot zero.\n", element1->threadNumber);
						}
						putItIn(element1, 0);
						element1 = NULL;
						continue;
					}
				}
			}
			counter++;
			success = 0;
		}

	}
	
}