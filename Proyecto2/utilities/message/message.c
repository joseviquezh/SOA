#include "message.h"

#include <stdio.h>

void printMessage (Message * message) {
    printf ("Message producer id = %i \n", message->producerId);
    printf ("Message key = %i \n", message->key);
    printf ("Message stop = %i \n", message->stop);

    struct tm tm = *localtime(&message->createdAt);
    printf("Message created at: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}