#include "globals.h"

//Adds an event to the proper position in the LinkedList, and returns 1 if successful, 0 if failed.
/*int addEvent(Link *sPtr, Link elementBeingAdded){
	Link newPtr, previousPtr, currentPtr;
	//check to make sure we have room in memory
		//add the pieces of information to the pointer
	newPtr->nextInList = NULL;
	previousPtr = NULL;
	currentPtr = *sPtr;
	//Brings us to the correct position in the list for this item.
	while (currentPtr!=NULL && arrivalTime > currentPtr->arrivalTime){
		previousPtr = currentPtr;
		currentPtr = currentPtr->nextInList;
	}
	//Are we at the beginning?
	if(previousPtr == NULL){
		newPtr->nextInList = *sPtr;
		*sPtr = newPtr;
		return 1;
	}
	//Now we're adding it to somewhere in the list
	else{
		previousPtr->nextInList = newPtr;
		newPtr->nextInList = currentPtr;
		return 1;
	}
}*/

//Brings the node to the back of the linkedlist given the node and the starting process.
void bringToBack(Link node, Link *sPtr){
	Link previousPtr, currentPtr;
	previousPtr = NULL;
	currentPtr = *sPtr;
	//Brings us to the correct position in the list for this item.
	while (currentPtr->thisLevel != UNDEFINED){
		previousPtr = currentPtr;
		currentPtr = currentPtr->next;
	}
	//Are we at the beginning?
	if(previousPtr == NULL){
		node->next = currentPtr;
		*sPtr = node;
	}
	//Now we're adding it to the end in the list
	else{
		previousPtr->next = node;
		node->next = currentPtr;
	}
}



//Creates a node based off of the provided information
NODE createNode(enum securityLevel thisLevel, int threadNumber){
	NODE *nextInList = NULL;
	pthread_t thread = NULL;
	NODE thisNode = {thread, thisLevel, threadNumber, nextInList};
	if (DEBUG && thisLevel != UNDEFINED){
		printf("Node has been created.\n");
	}
	return thisNode;
}

//Creates a link from a node
Link createLink(NODE thisNode){
	Link tempPtr;
	tempPtr = malloc(sizeof(NODE));
		//Memory check
		if (tempPtr){
			tempPtr->thread = thisNode.thread;
			tempPtr->thisLevel = thisNode.thisLevel;
			tempPtr->threadNumber = thisNode.threadNumber;
			tempPtr->next = thisNode.next;
			return tempPtr;
		}
		//Memory check failed case
		else{
			printf("There isn't enough memory to proceed");
			return tempPtr;
		}
}

//Takes the first event in the list, removing it from the linkedlist entirely, and returns that event.
Link getNextEvent(Link *sPtr){
	Link tempPtr;
	tempPtr = *sPtr;
	*sPtr = (*sPtr)->next;
	return tempPtr;
}

int isEmpty(Link *sPtr){
	Link tempPtr;
	tempPtr = *sPtr;
	if (tempPtr->thisLevel == UNDEFINED){
		return 1;
	}
	return 0;
}



