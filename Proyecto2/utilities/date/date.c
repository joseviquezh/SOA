#include "date.h"

time_t getCurrentDateTime () {
    time_t now;
    time(&now);

    return now;
}