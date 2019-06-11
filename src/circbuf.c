/******************************************************************************

                  Copyright (c) 2018 EmbedJournal

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

    Author : Siddharth Chandrasekaran
    Email  : siddharth@embedjournal.com
    Date   : Fri Aug 10 11:16:27 IST 2018

******************************************************************************/

#include <string.h>

#include "circbuf.h"

int __circ_gbuf_pop(circ_gbuf_t *circ_buf, void *elem, int read_only)
{
    int total;
    char *tail;

    total = circ_buf->push_count - circ_buf->pop_count;
    if (total < 0)
        total += (2 * circ_buf->size);

    if (total == 0)
        return -1; // Empty

    tail = (char *)circ_buf->buffer + ((circ_buf->pop_count % circ_buf->size)
                                            * circ_buf->element_size);

    if (elem)
        memcpy(elem, tail, circ_buf->element_size);

    if (!read_only) {
#ifdef CRICBUF_CLEAN_ON_POP
        memset(tail, 0, circ_buf->element_size);
#endif
        circ_buf->pop_count++;
        if (circ_buf->pop_count >= (2*circ_buf->size))
            circ_buf->pop_count = 0;
    }
    return 0;
}

int __circ_gbuf_push(circ_gbuf_t *circ_buf, void *elem)
{
    int total;
    char *head;

    total = circ_buf->push_count - circ_buf->pop_count;
    if (total < 0)
        total += (2 * circ_buf->size);

    if (total >=  circ_buf->size)
        return -1; // Full

    head = (char *)circ_buf->buffer + ( (circ_buf->push_count % circ_buf->size)
                                * circ_buf->element_size );
    memcpy(head, elem, circ_buf->element_size);
    circ_buf->push_count++;
    if (circ_buf->push_count >= (2*circ_buf->size))
        circ_buf->push_count = 0;
    return 0;
}

int __circ_gbuf_free_space(circ_gbuf_t *circ_buf)
{
    int total;

    total = circ_buf->push_count - circ_buf->pop_count;
    if (total < 0)
        total += (2 * circ_buf->size);

    return circ_buf->size - total;
}
