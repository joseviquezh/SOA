#include "circ_buff.h"

#define ENTRY_SIZE 256

struct circ_buff {
    int *data;
    size_t head;
    size_t tail;
    size_t max; //of the buffer
    bool full;
};

static void advance_pointer(cbuf_p cbuf)
{
    assert(cbuf);

    if(cbuf->full)
       {
        cbuf->tail = (cbuf->tail + 1) % cbuf->max;
    }

    cbuf->head = (cbuf->head + 1) % cbuf->max;
    cbuf->full = (cbuf->head == cbuf->tail);
}

static void retreat_pointer(cbuf_p cbuf)
{
    assert(cbuf);

    cbuf->full = false;
    cbuf->tail = (cbuf->tail + 1) % cbuf->max;
}

bool circ_buff_empty(cbuf_p cbuf)
{
    assert(cbuf);

    return (!cbuf->full && (cbuf->head == cbuf->tail));
}

int circ_buff_get(cbuf_p cbuf, int * data)
{
    assert(cbuf && data && cbuf->data);

    int r = -1;

    if(!circ_buff_empty(cbuf))
    {
        *data = cbuf->data[cbuf->tail];
        retreat_pointer(cbuf);

        r = 0;
    }

    return r;
}

void circ_buff_set(cbuf_p cbuf, int data)
{
    assert(cbuf && cbuf->data);

    cbuf->data[cbuf->head] = data;

    advance_pointer(cbuf);
}

void circ_buff_free(cbuf_p cbuf)
{
    assert(cbuf);
    free(cbuf);
}

void circ_buff_reset(cbuf_p cbuf)
{
    assert(cbuf);

    cbuf->head = 0;
    cbuf->tail = 0;
    cbuf->full = false;
}

cbuf_p circ_buff_init(int *data, size_t size)
{

    cbuf_p cbuf = malloc(sizeof(circ_buff));
    assert(cbuf);

    cbuf->data = data;
    cbuf->max = size;
    /* Ensure to create a buffer in empty state */
    circ_buff_reset(cbuf);

    assert(circ_buff_empty(cbuf));

    return cbuf;
}

int main(int argc, char *argv[])
{
    int * data  = malloc(BUFFER_SIZE * sizeof(int));
    cbuf_p cbuf = circ_buff_init(data,BUFFER_SIZE);

    int message  = 101;
    int message_read;

    circ_buff_set(cbuf, message);

    for(int i = 0; i < BUFFER_SIZE+3; i++)
    {
        circ_buff_set(cbuf, i);
        printf("Write: %d\n", i);
    }

    while(!circ_buff_empty(cbuf))
    {
        int data;
        circ_buff_get(cbuf, &data);
        printf("Read: %d\n", data);
    }
        
    free(data);
    circ_buff_free(cbuf);

    return 0;
}
