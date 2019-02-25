// System includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Custom includes
#include "table.h"
#include "syscall.h"

int currentLength = 0;

struct sysCallItem {
    char *name;
    int value;
};

#define MAXITEMS 1000
static struct sysCallItem* sysCallsDictionary[MAXITEMS];

// Public function to add or update a syscall call, if syscall is not already registered a new record is added to the list
int addRecord (long pCall) {
    if (currentLength == 0) {
        addItemToDictionary (callname(pCall));
    } else {

        char * name = callname(pCall);
        int index = getIndexOfItem (name);

        if (index >= 0) {
            struct sysCallItem *item = sysCallsDictionary[index];
            item->value = item->value + 1;
        } else {
            addItemToDictionary (name);
        }
    }
}

// Helper to a new item to the dictionary
int addItemToDictionary (char * pCallName) {
    struct sysCallItem *newItem;
    newItem = (struct sysCallItem *) malloc(sizeof(*newItem));
    newItem->name = pCallName;
    newItem->value = 1;

    sysCallsDictionary[currentLength] = newItem;
    currentLength = currentLength + 1;
}

// Helper to add an item to the list
int getIndexOfItem (char* pName){
    int index = 0;
    struct sysCallItem *item;
    while (index < currentLength){
        item = sysCallsDictionary[index];
        if (strcmp(item->name, pName) == 0) return index;
        index = index + 1;
    }
    return -1;
}

// Public method to print in standard output the content of the list
void printTable () {
    struct sysCallItem *item;
    printf("\n\nSyscalls called overview\n");
    for (int index = 0; index < currentLength; index++) {
        item = sysCallsDictionary[index];
        printf("%s -> %i\n", item->name, item->value);
    }
}