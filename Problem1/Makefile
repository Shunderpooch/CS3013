##Start of Makefile
make: main clean

main: FCFS.o nodeOperations.o cluster.o
	gcc -o main FCFS.o nodeOperations.o cluster.o -pthread -lm
	
FCFS.o: FCFS.c globals.h
	gcc -Wall -c -pthread FCFS.c

nodeOperations.o: nodeOperations.c globals.h
	gcc -Wall -c nodeOperations.c

cluster.o: cluster.c globals.h
	gcc -Wall -c cluster.c

clean: 
	rm *.o

##End of Makefile