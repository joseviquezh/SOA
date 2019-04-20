#ifndef MESSAGE_H_
#define MESSAGE_H_

// System includes
#include <time.h>

typedef struct {
    int producerId;
    int key;
    int stop;
    time_t createdAt;
} Message;

void printMessage (Message * message);
#endif //MESSAGE_H_