#include "circ_buff.h"
#include "utilities/message/message.h"

#define ENTRY_SIZE 256

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
int circ_buff_get(cbuf_p cbuf, Message * data)
{
    assert(cbuf && data && cbuf->data);

    int r = -1;

    if(!circ_buff_empty(cbuf))
    {
        memcpy(data, &cbuf->data[cbuf->tail], sizeof(Message));
        retreat_pointer(cbuf);

        r = 0;
    }

    return r;
}

void circ_buff_set(cbuf_p cbuf, Message data)
{
    assert(cbuf && cbuf->data);

    memcpy(&cbuf->data[cbuf->head], &data, sizeof(Message));

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

cbuf_p circ_buff_init(cbuf_p cbuf, size_t size)
{

    assert(cbuf);

    cbuf->max = size;
    /* Ensure to create a buffer in empty state */
    circ_buff_reset(cbuf);

    assert(circ_buff_empty(cbuf));

    return cbuf;
}
