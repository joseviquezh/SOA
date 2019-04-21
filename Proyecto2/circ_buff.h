#ifndef CIRC_BUFF_H_
#define CIRC_BUFF_H_

#include <stdio.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "utilities/message/message.h"

#define BUFFER_SIZE 4096
#define MESSAGE_SIZE 64

typedef struct circ_buff circ_buff;
typedef circ_buff* cbuf_p;

struct circ_buff {
    Message data [BUFFER_SIZE];
    size_t head;
    size_t tail;
    size_t max; //of the buffer
    bool full;
    bool stop;
    int consumersAlive;
    int producersAlive;

};

static void advance_pointer(cbuf_p cbuf);

static void retreat_pointer(cbuf_p cbuf);

bool circ_buff_empty(cbuf_p cbuf);

int circ_buff_get(cbuf_p cbuf, Message * data);

void circ_buff_set(cbuf_p cbuf, Message data);

void circ_buff_free(cbuf_p cbuf);

void circ_buff_reset(cbuf_p cbuf);

cbuf_p circ_buff_init(cbuf_p cbuf, size_t size);

#endif //CIRC_BUFF_H_
