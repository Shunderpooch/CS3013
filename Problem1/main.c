//Arthur Dooner & Arun Donti, CS 3013
//Program 3
//#include "globals.h"


//Operates on the first item in the Processor Quene, dynamically for FCFS and RR. Returns the time used by the CPU across operating on this process.
/*int processorSimulation(Link *sProcPtr, int timeSlice, int systemTimeIHMs){
	//Gets the event at the head of the processor simulation to work on.
	Link node = getNextEvent(sProcPtr);
	if (DEBUG){
		printf("The processor is currently working on:\n");
		printNode(node);
	}
	return 1;
	//If the timeslice is equal to or greater than the node at the front process' remaining CPU time, then we can remove it from the list and return the time needed to run.
	/*if (timeSlice >= (node->CPUTime)){
		int CPUTime = (node->CPUTime);
		double TotalTime = (((double)systemTimeIHMs + CPUTime - (node->arrivalTime))/10);
		printf("\nProcess %4d has completed in %3lf seconds. It started at %3lf and ended at %3lf. \n", node->processID, TotalTime, ((double)node->arrivalTime)/10, ((double)systemTimeIHMs/10);
		calcMeanAndVariance(TotalTime);
		free(node);
		return CPUTime;
	//}
	//Otherwise, we remove time from the CPU time on the node and bring the node to the back. 
	else{
		//node->CPUTime -= timeSlice;
		if (DEBUG){
			printf("Process %4d has been worked at for %lf seconds. The processor is moving it to the back and starting the next.\n", node->processID, (((double)timeSlice)/10));
		}
		if(int bringBack==1){
			bringToBack(node, sProcPtr);
		}
		return timeSlice;
	} 
} */

//Dynamically gathers all of the needed times for each event and upon given the sentinel, calculates and prints out the mean and variance.
/*void calcMeanAndVariance(double newTime){
	static int counter = 0;
	double sum = 0;
	double varianceSum, variance, mean;

	if (newTime == -1){
		int i;
		for (i = 0; i < numberOfProcesses; i++){
			sum += processTimes[i];
		}
		mean = sum / numberOfProcesses;
		int j;
		for (j = 0; j < numberOfProcesses; j++){
			varianceSum+= pow((processTimes[j]-mean), 2);
		}
		variance=varianceSum/(numberOfProcesses-1);
		printf("Mean of this data: %lf Variance: %lf\n", mean, variance);
	}
	else{
		processTimes[counter]=newTime;
		counter++;
		if (DEBUG){
			printf("The time %lf seconds was added to the list at position %d\n", newTime, counter);
		}
	}

}
//Takes in a certain amount of lines of command line input, adds them all as nodes to the list, and removes the first two from the list
//prints them, and finally prints the rest of the list. 
/*
int main(){
    //Start of the EventList
    Link sPtr = NULL;
    //Start of the Processor Quene
    Link sProcPtr = NULL;
	FCFS(&sPtr, &sProcPtr);
	return 1;
}
*/
