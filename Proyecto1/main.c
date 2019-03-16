#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>

#include "piApproximation.c"
#include "timer/timer.h"

#define NUM_THREADS 5
void scheduler();

typedef struct{
	int id;                  // Thread ID
    int mode;				 // 0 expropiative, 1 Non expropiative
	double result;			 // pi calculation result/progress
    int executed;			 // To know if the thread has been executed yet
    int piTerms;             // PI terms to calculate depending on the selected working units. 1 woking unit == 50 piTerms
    double workPercentage;   // Between 0-100%
	int numTickets;          // Number of assigned tickets
    int* tickets;            // Lottery tickets
	sigjmp_buf buffer;       // Thread buffer
	int finnished;
} Thread;


int QUANTUM_SIZE = 0;        // Defined in miliseconds
int NUM_TICKETS = 0;            // Total ammount of tickets to be assigned
Thread* THREADS[NUM_THREADS];
Thread* runningThread;
sigjmp_buf parent;

/*
    Callback that is fired when the timer's time is up.
*/
void timerHandler(int pSig)
{
    printf("DEBUG: Time is up!!! \n");
    siglongjmp(parent, 1);
}


void swap (int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}


void calculatePi(){
	runningThread->executed = 1;
	sigsetjmp(runningThread->buffer, 1);

	LookUp* ptrPiAproximation = getInitState();
	sigsetjmp(runningThread->buffer, 1);

    double progress; // percent
	sigsetjmp(runningThread->buffer, 1);

	long long int calculatedTerms,termsToCalculate;
	sigsetjmp(runningThread->buffer, 1);

	if(runningThread->mode == 0){
		progress=0.0;
		sigsetjmp(runningThread->buffer, 1);

	}
	else{
		calculatedTerms = 0;
		termsToCalculate = (runningThread->piTerms * 100 )/runningThread->workPercentage;
	}

	/*
		Expropiative: do work during a certaing amount of time
		Non-expropiative: do a specific work percentage
	*/

	for(int piTerm = 0; piTerm < runningThread->piTerms; ++piTerm){
		pi_gregory_pauseable_2(ptrPiAproximation);
		sigsetjmp(runningThread->buffer, 1);

		if(runningThread->mode == 1 && calculatedTerms++ == termsToCalculate){
			if(sigsetjmp(runningThread->buffer, 1) == 0) siglongjmp(parent, 1);
			calculatedTerms = 0;
		}
	}

	/*
	double progress=0.0;// percent

	long long int fractionValue,fractionValueAdjusted,totalWork,i;
	LookUp* ptrPiAproximationExpro = getInitState();
	LookUp* ptrPiAproximationNonExpro = getInitState();

    if(runningThread->mode == 0){
        if(runningThread->piTerms>0){
			pi_gregory_pauseable(runningThread->piTerms*MIN_OF_WORK,ptrPiAproximationExpro);
			//The value of pi is saved in ptrPiAproximationExpro->piSoFar
			progress=ptrPiAproximationExpro->iterations*100/(runningThread->piTerms*MIN_OF_WORK);
			sigsetjmp(runningThread->buffer, 0);
			printf("Result so far %d\n", runningThread->result);
		}
        }else{
            printf("\nIncorrect parameters for: piTerms: %d\n",runningThread->piTerms);
        }
    }
    else{
        if(validateParaetersNoExpropiatives(runningThread->piTerms,runningThread->workPercentage)){
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
                if(sigsetjmp(runningThread->buffer, 1) == 0) siglongjmp(parent, 1);
            }
            // TO CONSULT FINAL VULE OF PI             ->     ptrPiAproximationNOExpro->piSoFar
            // TO CONSULT THE TOTAL ITERATIONS DONE    ->     ptrPiAproximationNOExpro->iterations
        }else{
            printf("\nIncorrect parameters for: percentage: %f or piTerms: %d\n",runningThread->workPercentage,runningThread->piTerms);
        }
    }
	*/
	printf("DEBUG: Process %d ended its execution\n", runningThread->id);
	sigsetjmp(runningThread->buffer, 1);
	runningThread->finnished = 1;
	sigsetjmp(runningThread->buffer, 1);
	runningThread->result = ptrPiAproximation->piSoFar;
	sigsetjmp(runningThread->buffer, 1);
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
        printf("\nDEBUG: Selecting a wining ticket\n");
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
		if(threadId < NUM_THREADS){
			printf("DEBUG: The winning ticket is %d and the winner is %d\n", ticket, threadId);
	        runningThread = THREADS[threadId];
			if(runningThread->finnished == 0){
				if(sigsetjmp(parent, 1) == 0){
					if(runningThread->mode == 0){
						setUpTimer(timerHandler);
						setTimer(QUANTUM_SIZE);
					}
					if(runningThread->executed){
						printf("DEBUG: The proces was already executed\n");
			            siglongjmp(runningThread->buffer, threadId);
			        }
			        else{
						printf("DEBUG: The proces wasn't already executed\n");
			            calculatePi();
			        }
				}
			}
			else{
				printf("DEBUG: The proces already finnished its calculations\n");
			}
		}
		else{
			--i;
		}
    }
	printf("\nAll the tickets were chosen, here are the results:\n");
	for(int threadId = 0; threadId < NUM_THREADS; ++threadId){
		printf("    %d: %lf\n",threadId, THREADS[threadId]->result);
	}
}


int main(int argc, char *argv[]){
    int mode = 1;
    do{
        printf("How should the threads work? 0 == Expropiative 1 == Non-expropiative: ");
        scanf ("%d",&mode);
    }while(mode != 0 && mode !=1);

    double workPercentage= 0;
    if(mode == 1){
        do{
            printf("Enter the work percentage at wich the thread will stop its execution. Mimimum 1: ");
            scanf ("%lf",&workPercentage);
        }while(workPercentage < 1);
    }

    int piTerms= 0;
    do{
        printf("Enter the work units for each thread. Minimum 1: ");
        scanf ("%d",&piTerms);
    }while(piTerms < 1);

	int quantum= 0;
	if(mode == 0){
		do{
	        printf("Enter the quantum size in miliseconds: ");
	        scanf ("%d",&quantum);
	    }while(quantum < 1);
	}

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
        THREADS[thread]->piTerms = piTerms*50;
        THREADS[thread]->numTickets = tickets;
        THREADS[thread]->tickets = malloc(tickets * sizeof(int));
        NUM_TICKETS += tickets;
		QUANTUM_SIZE = quantum;
    }

    // Call the scheduler to strart the program
    scheduler();

    // Free allocatied memory
    for(int thread = 0; thread < NUM_THREADS; ++thread){
        free(THREADS[thread]->tickets);
    }

    return(0);
}
