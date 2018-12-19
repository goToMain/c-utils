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

#include "circular-generic-buffer.h"

static inline void zero_pad(char *s,int len)
{
    int i;
    for(i=strnlen(s,len); i<len; i++)
        s[i] = 0;
}

int circ_gbuf_pop(circ_gbuf_t *circ_buf, void *elem, int read_only)
{
    char *tail;

    if ((circ_buf->push_count - circ_buf->pop_count) == 0)
        return -1;    // quit with an error

    tail = circ_buf->buffer + ( (circ_buf->pop_count % circ_buf->size)
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

int circ_gbuf_push(circ_gbuf_t *circ_buf, void *elem)
{
    char *head;

    if ((circ_buf->push_count - circ_buf->pop_count) >=  circ_buf->size)
        return -1;

    head = circ_buf->buffer + ( (circ_buf->push_count % circ_buf->size)
                                * circ_buf->element_size );
    memcpy(head, elem, circ_buf->element_size);
    circ_buf->push_count++;
    if (circ_buf->push_count >= (2*circ_buf->size))
        circ_buf->push_count = 0;
    return 0;
}

int circ_gbuf_free_space(circ_gbuf_t *circBuf)
{
    return circBuf->size - (circBuf->pop_count - circBuf->push_count);
}

#ifdef C_UTILS_TESTING
/* To test this module,
 * $ gcc -Wall -DC_UTILS_TESTING circular-generic-buffer.c
 * $ ./a.out
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct my_struct {
    int a;
    char b;
    float c;
};

CIRC_GBUF_DEF(struct my_struct, my_circ_buf, 32);

int main()
{
    int i;
    struct my_struct a[20];

    srand(time(NULL));

    for (i=0; i<10; i++) {
        a[i].a = rand();
        if (CIRC_GBUF_PUSH(my_circ_buf, &a[i])) {
            printf("Out of space in CB at %d\n", i);
            return -1;
        }
    }

    for (i=0; i<10; i++) {
        if (CIRC_GBUF_POP(my_circ_buf, &a[10 + i])) {
            printf("CB is empty at %d\n", i);
            return -1;
        }
        if (a[10 + i].a != a[i].a) {
            printf("Invalid data at %d\n", 10 + i);
            return -1;
        }
    }

    printf("Test Passed %d %d\n", my_circ_buf.pop_count, my_circ_buf.push_count);

    return 0;
}

#endif
