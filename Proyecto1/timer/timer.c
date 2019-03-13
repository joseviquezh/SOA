// System includes
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>

// Custom includes
#include "timer.h"

// Properties.

struct itimerval timer;
int timeUnit = 1; // Must be 1000 to be milliseconds, 1 only for testing purposes.
void (*callback)(int);

/* 
    Configures the timer.
*/
void setUpTimer (void (*pCallback)(int)) {
    printf("Initializing timer\n");
    
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = 0;

    callback = pCallback;
}

/*
    Sets the timer and fires it.
*/
void setTimer (int pTime) {
    printf("Setting timer\n");
    
    signal(SIGALRM, callback);
    
    timer.it_value.tv_usec = pTime * timeUnit;
    setitimer(ITIMER_REAL, &timer, 0);
}