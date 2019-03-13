#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>
#include "piApproximation.c"

#define NUM_THREADS 5
void scheduler();

typedef struct{
	int id;
    int mode;              // 0 expropiative, 1 Non expropiative
	int result;            // pi calculation result/progress
    int executed;          // To know if the thread has been executed yet
    int workUnits;         // Amount of work units
    int workPercentage;    // Between 0-100%
	int numTickets;        // Number of assigned tickets
    int* tickets;          // Lottery tickets
	sigjmp_buf buffer;     // Thread buffer
} Thread;


int QUANTUM_SISE = 1000;        // Defined in miliseconds
int NUM_TICKETS = 0;            // Total ammount of tickets to be assigned
Thread* THREADS[NUM_THREADS];
Thread* runningThread;
sigjmp_buf parent;


void swap (int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}


void calculatePi(){
    double progress=0.0;// percent
    long long int i,fractionValue,fractionValueAdjusted,totalWork;
    LookUp* ptrPiAproximationExpro=getInitState();
    LookUp* ptrPiAproximationNOExpro=getInitState();
    

    runningThread->executed = 1;
    if(runningThread->mode == 0){
        /*
            Expropiative: do work during a certaing amount of time

            sigsetjmp -> Saves a checkpoint (stored in runningThread->buffer)
            siglongjmp -> Moves to a checkpoint (stored in parent)
        */
        /*
            // Since the time could end at any point, need to continously save the progress
            while(time){
                // Do calculations
                runningThread->result = result;
                sigsetjmp(runningThread->buffer, 1);

                // Do calculations
                runningThread->result = result;
                sigsetjmp(runningThread->buffer, 1);

                // Do calculations
                */
            if(runningThread->workUnits>0){
                pi_gregory_pauseable(runningThread->workUnits*MIN_OF_WORK,ptrPiAproximationExpro);
                //The value of pi is saved in ptrPiAproximationExpro->piSoFar
                progress=ptrPiAproximationExpro->iterations*100/(runningThread->workUnits*MIN_OF_WORK);
            }else{
                printf("Incorrect parameters for: workunits: %d",runningThread->workUnits);
            }
               /*
            }
            runningThread->result = result;     // Save result in the object
            siglongjmp(parent, 1);      // Move back to the scheduler
        */
    }
    else{
        /*
            Non-expropiative: do a specific work percentage
        */
        if(validateParaetersNoExpropiatives(runningThread->workUnits,runningThread->workPercentage)){
            fractionValue=totalWork*runningThread->workPercentage/100;
            fractionValueAdjusted=fractionValue;
            i=(long long int)totalWork/fractionValue;
            printf("Total work %lld processing %lld per %lld iteration \n",totalWork,fractionValue,i);
            while(i-->=0&&fractionValueAdjusted>0){
                pi_gregory_pauseable(fractionValueAdjusted,ptrPiAproximationNOExpro);
                //TO PROGRESS PERCENTAGE UNTIL NOW     ->     i+1>0?(long long int)((totalWork/fractionValue-i)*runningThread->workPercentage):100
                //VALUE OF PI UNTIL NOW                ->     ptrPiAproximationNOExpro->piSoFar
                if(fractionValueAdjusted>totalWork-ptrPiAproximationNOExpro->iterations)
                fractionValueAdjusted=totalWork-ptrPiAproximationNOExpro->iterations;
                printf("This line has be replaced with the command required to release the thread.\n");
            }
            // TO CONSULT FINAL VULE OF PI             ->     ptrPiAproximationNOExpro->piSoFar
            // TO CONSULT THE TOTAL ITERATIONS DONE    ->     ptrPiAproximationNOExpro->iterations
        }else{
            printf("Incorrect parameters for: percentage: %d or workunits: %d",runningThread->workPercentage,runningThread->workUnits);
        }
        //The final value of pi is saved in ptrPiAproximationExpro->piSoFar
        /*
            sigsetjmp(runningThread->buffer, 1);    // Save chackpoint so we can return later
            while(runningThread->workPercentage > completedWorkPercentage){
                // Do calculations
            }
            runningThread->result = result;     // Save result in the object
            siglongjmp(parent, 1);      // Move back to the scheduler
        */
    }

    for(int i = 0; i < 10; ++i){
        if(sigsetjmp(runningThread->buffer, 1) == 0) siglongjmp(parent, 1);
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
    for(int threadId = 0; threadId < NUM_THREADS; ++threadId){
        startIndex = endIndex;
        endIndex += THREADS[threadId]->numTickets;
        for(int i = 0; i < THREADS[threadId]->numTickets; ++i){
            THREADS[threadId]->tickets[i] = tickets[startIndex++];
        }
    }


    // Shuffle the tickts array
    for (int i = NUM_TICKETS-1; i > 0; i--){
        int j = rand() % (i+1);
        swap(&tickets[i], &tickets[j]);
    }

    // Randomly select a winning ticket until there are no tickets
    for(int i = 0; i < NUM_TICKETS; ++i){
        printf("Selecting a wining ticket\n");
        int ticket = tickets[i];
        int threadId;
        int winnerFound = 0;
        for(threadId = 0; threadId < NUM_THREADS; ++threadId){
            for(int j = 0; j < THREADS[threadId]->numTickets; ++j){
                if (THREADS[threadId]->tickets[j] == ticket){
                    winnerFound = 1;
                    break;
                }
            }
            if(winnerFound) break;
        }
        printf("The winning ticket is %d and the winner is %d\n", ticket, threadId);
        runningThread = THREADS[threadId];
        if(runningThread->executed){
            if(sigsetjmp(parent, 1) == 0) siglongjmp(THREADS[threadId]->buffer, threadId);
        }
        else{
            if(sigsetjmp(parent, 1) == 0) calculatePi();
        }
    }
}


int main(int argc, char *argv[]){
    int mode = 1;
    do{
        printf( "How should the threads work? 0 == Expropiative 1 == Non-expropiative: ");
        scanf ("%d",&mode);
    }while(mode != 0 && mode !=1);

    int workPercentage= 0;
    if(mode == 0){
        do{
            printf( "Enter the work percentage at wich the thread will stop its execution. Mimimum 1: ");
            scanf ("%d",&workPercentage);
        }while(workPercentage < 1);
    }

    int workUnits= 0;
    do{
        printf( "Enter the work units for each thread. Minimum 50: ");
        scanf ("%d",&workUnits);
    }while(workUnits < 50);

    // Initialize all the threads
    for(int thread = 0; thread < NUM_THREADS; ++thread){
        THREADS[thread] = (Thread*) malloc(sizeof(Thread));

        // Know the amount of tickets per thread
        int tickets;
        do{
            printf("Enter the amount of tickets for thread %d: ", thread);
            scanf ("%d",&tickets);
        }while(tickets < 1);

        THREADS[thread]->id = thread;
        THREADS[thread]->mode = mode;
        THREADS[thread]->result = 0;
        THREADS[thread]->executed = 0;
        THREADS[thread]->workPercentage = workPercentage;
        THREADS[thread]->workUnits = workUnits;
        THREADS[thread]->numTickets = tickets;
        THREADS[thread]->tickets = malloc(tickets * sizeof(int));
        NUM_TICKETS += tickets;
    }

    // Call the scheduler to strart the program
    scheduler();

    // Free allocatied memory
    for(int thread = 0; thread < NUM_THREADS; ++thread){
        free(THREADS[thread]->tickets);
    }

    return(0);
}
