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
    Date   : Fri Aug 10 11:22:11 IST 2018

******************************************************************************/
#ifndef __CIRCULAR_GENERIC_BUFFER_H_
#define __CIRCULAR_GENERIC_BUFFER_H_

#include <stdint.h>

typedef struct {
    void * const buffer;
    uint16_t head;
    uint16_t tail;
    uint16_t const size;
    uint16_t const element_size;
} circ_gbuf_t;

#define CIRC_GBUF_V_DEF(t,x,s)                 \
    t x ## _circ_gbuf_data[s];                 \
    circ_gbuf_t x = {                          \
        .buffer = x ## _circ_gbuf_data,        \
        .head =0,                              \
        .tail =0,                              \
        .size = s ,                            \
        .element_size = sizeof(t)              \
    };

#define CIRC_GBUF_DEF(t,x,s)                   \
    CIRC_GBUF_V_DEF(t,x,s)                     \
    inline int x##_push_refd(t * pt)           \
    {                                          \
        return circ_gbuf_push(&x,pt);          \
    }                                          \
    inline int x##_pop_refd(t * pt)            \
    {                                          \
        return circ_gbuf_pop(&x,pt,0);         \
    }                                          \
    inline int x##_peek_refd(t * pt)           \
    {                                          \
        return circ_gbuf_pop(&x,pt,1);         \
    }

#define CIRC_GBUF_PUSH(x,y) x ## _push_refd(y)
#define CIRC_GBUF_PEEK(x,y) x ## _peek_refd(y)
#define CIRC_GBUF_POP(x,y) x ## _pop_refd(y)
#define CIRC_GBUF_FS(x) circ_gbuf_free_space(&x)

int circ_gbuf_push(circ_gbuf_t *circ_gbuf, void *elem);
int circ_gbuf_pop (circ_gbuf_t *circ_gbuf, void *elem, int read_only);
int circ_gbuf_free_space(circ_gbuf_t *circ_gbuf);

#endif
