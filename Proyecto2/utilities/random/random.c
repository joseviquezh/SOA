#include "random.h"

#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

int generateRandom(int lower, int upper) 
{ 
    srand(time(NULL));
    return (rand() % (upper - lower + 1)) + lower; 
} 

int generateRandomKey() 
{ 
    return generateRandom(1, 5); 
} 