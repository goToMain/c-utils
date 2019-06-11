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
    Date   : Sun Aug  5 09:42:31 IST 2018

******************************************************************************/
#ifndef __CIRCULAR_BYTE_BUFFER_H_
#define __CIRCULAR_BYTE_BUFFER_H_

#include <stdint.h>

typedef struct {
    uint8_t * const buffer;
    int head;
    int tail;
    const int maxlen;
} circ_bbuf_t;

#define CIRC_BBUF_DEF(x,y)                \
    uint8_t x##_data_space[y+1];          \
    circ_bbuf_t x = {                     \
        .buffer = x##_data_space,         \
        .head = 0,                        \
        .tail = 0,                        \
        .maxlen = y+1                     \
    }

/*
 * Method: circ_buf_pop
 * Returns:
 *  0 - Success
 * -1 - Empty
 */
int circ_bbuf_pop(circ_bbuf_t *c, uint8_t *data);

/*
 * Method: circ_buf_push
 * Returns:
 *  0 - Success
 * -1 - Out of space
 */
int circ_bbuf_push(circ_bbuf_t *c, uint8_t data);

/*
 * Method: circ_bbuf_free_space
 * Returns: number of bytes available
 */
int circ_bbuf_free_space(circ_bbuf_t *c);

#endif /* __CIRCULAR_BYTE_BUFFER_H_ */
