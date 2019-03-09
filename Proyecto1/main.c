#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>

void scheduler();

int NUM_THREADS = 6;            // Number of threads to work with
int EXPROPIATIVE_MODE = 0;      // Operation move for each thread
int QUANTUM_SISE = 1000;        // Defined in miliseconds
int NUM_TICKETS = 0;            // Total ammount of tickets to be assigned
int WORKLOAD = 0;               // Workload for each thread
jmp_buf *THREADS_BUFFER;        // Buffer for each thread. 0 is the parent
int *THREADS_RESULTS;           // Results for each thread calculation. Starting from 1, since the parent only manages
int *THREADS_TICKETS;           // Tickets of each thread. Starting from 1, since the parent only manages
int *THREADS_NUM_TICKETS;       // Amount of tickets per thread. Starting from 1, since the parent only manages

void swap (int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}


void doWork(int threadId){
    // Initialize the threads context
    printf("Initializing thread %d\n", threadId);
    THREADS_RESULTS[threadId] = 0;
    threadId = sigsetjmp(THREADS_BUFFER[threadId], 1);
    if(threadId == 0) siglongjmp(THREADS_BUFFER[0], 1);

    // Do the calculations in the defined quantum
    for(int i = 0; i < 10; ++i){
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
    // Create an array with all the ticket numbers
    int tickets[NUM_TICKETS];
    for(int i = 0; i < NUM_TICKETS; ++i){
        // Choose a random and unique number
        tickets[i] = i + 1;
    }

    // Shuffle the tickts array
    srand(time(NULL));
    for (int i = NUM_TICKETS-1; i > 0; i--){
        int j = rand() % (i+1);
        swap(&tickets[i], &tickets[j]);
    }

    // Assign the corresponding amount of tickets to each thread
    int startIndex = 0, endIndex = 0;
    for(int threadId = 1; threadId < NUM_THREADS; ++threadId){
        startIndex = endIndex;
        endIndex += THREADS_NUM_TICKETS[threadId];
        for(int i = 0; i < THREADS_NUM_TICKETS[threadId]; ++i){
            THREADS_TICKETS[threadId * NUM_TICKETS + i] = tickets[startIndex++];
        }
    }

    // Randomly select a winning ticket (at the moment, only select 5 winners)
    for(int i = 0; i < 5; ++i){
        printf("Selecting a wining ticket\n");
        int ticket = (rand() % (NUM_TICKETS - 1) ) + 1;
        int threadId;
        int winnerFound = 0;
        for(threadId = 1; threadId < NUM_THREADS; ++threadId){
            for(int j = 0; j < THREADS_NUM_TICKETS[threadId]; ++j){
                if (THREADS_TICKETS[threadId * NUM_TICKETS + j] == ticket){
                    winnerFound = 1;
                    break;
                }
            }
            if(winnerFound) break;
        }
        // printf("The winning ticket is %d and the winner is %d\n", ticket, threadId);
        if(sigsetjmp(THREADS_BUFFER[0], 1) == 0) siglongjmp(THREADS_BUFFER[threadId], threadId);
    }
}

int main(int argc, char *argv[]){
    // Know the amount of threads to be used
    int threads;
    do{
        printf( "Enter the amount of threads: ");
        scanf ("%d",&threads);
    }while(threads < 5);
    NUM_THREADS = threads + 1;

    // Initialize global variables depending on the user input
    THREADS_BUFFER = malloc(NUM_THREADS * sizeof(jmp_buf));
    THREADS_RESULTS = malloc(NUM_THREADS * sizeof(int));
    THREADS_NUM_TICKETS = malloc(NUM_THREADS * sizeof(int));

    // Know the amount of tickets per thread
    for(int i = 1; i < NUM_THREADS; ++i){
        int tickets;
        do{
            printf("Enter the amount of tickets for thread %d: ", i);
            scanf ("%d",&tickets);
        }while(tickets < 1);
        THREADS_NUM_TICKETS[i] = tickets;
        NUM_TICKETS += tickets;
    }

    THREADS_TICKETS = malloc(NUM_THREADS * NUM_TICKETS * sizeof(int));

    // Initialize all threads contexts
    for(int threadId = 1; threadId < NUM_THREADS; ++threadId){
        if(sigsetjmp(THREADS_BUFFER[0], 1) == 0) doWork(threadId);
    }

    // Call the scheduler to strart the program
    //scheduler();

    free(THREADS_BUFFER);
    free(THREADS_RESULTS);
    free(THREADS_NUM_TICKETS);
    free(THREADS_TICKETS);
    return(0);
}
