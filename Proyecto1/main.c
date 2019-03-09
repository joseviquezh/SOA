#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>

void scheduler();

int NUM_THREADS = 6;            // Number of threads to work with
int EXPROPIATIVE_MODE = 0;      // Operation move for each thread
int QUANTUM_SISE = 1000;        // Defined in miliseconds
int NUM_TICKETS = 1;            // Tickets assigned to each thread
int WORKLOAD = 0;               // Workload for each thread
jmp_buf *THREADS_BUFFER;        // Buffer for each thread. 0 is the parent
int *THREADS_RESULTS;           // Results for each thread calculation. Starting from 1, since the parent only manages
int *THREADS_TICKETS;          // Tickets of each thread. Starting from 1, since the parent only manages


void doWork(int threadId){
    // Initialize the threads context
    printf("Initializing thread %d\n", threadId);
    THREADS_RESULTS[threadId] = 0;
    threadId = sigsetjmp(THREADS_BUFFER[threadId], 1);
    if(threadId == 0) siglongjmp(THREADS_BUFFER[0], 0);

    // Do the calculations in the defined quantum
    for(int i = 0; i < 1000; ++i){
        // This is just an example of some work
        printf("Thread %d is doing some work\n", threadId);
        THREADS_RESULTS[threadId] += threadId;
        printf("    Thread %d result is %d\n", threadId, THREADS_RESULTS[threadId]);
        // End of the example

        // Once the quantum have expired, move back to the schedler context
        threadId = sigsetjmp(THREADS_BUFFER[threadId], 1);
        if(threadId == 0) siglongjmp(THREADS_BUFFER[0], threadId);
    }
}

void scheduler(){
    /*
        Is in charge of randomly assigning the lottery tickets to all the
        threads. It assign NUM_TICKETS to all the threads.
        After the assignation, the scheduler is in charge of randomly selecting
        a "winning ticket" and execute the threads that has that ticket.
    */
    srand(time(NULL));   // Initialization, should only be called once.

    // Assign lottery tickets to al threads
    for(int threadId = 1; threadId < NUM_THREADS; ++threadId){
        for(int numTicket = 0; numTicket < NUM_TICKETS; ++numTicket){
            THREADS_TICKETS[threadId * NUM_TICKETS + numTicket] = 0;        // Analog to THREADS_TICKETS[threadId][numTicket]
        }
    }

    // Randomly select a winning ticket
    for(int i = 0; i < 5; ++i){
        printf("Selecting a random thread\n");
        int threadId = (rand() % (NUM_THREADS - 1) ) + 1;
        if(sigsetjmp(THREADS_BUFFER[0], 1) == 0) siglongjmp(THREADS_BUFFER[threadId], threadId);
    }
}

int main () {
    // Initialize global variables depending on the user input
    THREADS_BUFFER = malloc(NUM_THREADS * sizeof(jmp_buf));
    THREADS_RESULTS = malloc(NUM_THREADS * sizeof(int));
    THREADS_TICKETS = malloc(NUM_THREADS * (NUM_TICKETS + 0) * sizeof(int));

    // Initialize all threads contexts
    for(int threadId = 1; threadId < NUM_THREADS; ++threadId){
        if(sigsetjmp(THREADS_BUFFER[0], 1) == 0) doWork(threadId);
    }

    // Call the scheduler to strart the program
    scheduler();

    free(THREADS_BUFFER);
    free(THREADS_RESULTS);
    free(THREADS_TICKETS);
    return(0);
}
