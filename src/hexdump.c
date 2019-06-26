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

#include <stdio.h>
#include <stdint.h>

void hexdump(const char *head, const uint8_t *data, int len)
{
    int i;
    char str[16];

    printf("%s [%d] =>\n    0000  %02x ", head, len, data[0]);
    str[0] = isprint(data[0]) ? data[0] : '.';
    for (i=1; i<len; i++) {
        if ((i&0x0f) == 0) {
            printf(" |%16s|", str);
            printf("\n    %04d  ", i);
        } else if((i&0x07) == 0) {
            printf(" ");
        }
        printf("%02x ", data[i]);
        str[i&0x0f] = isprint(data[i]) ? data[i] : '.';
    }
    if ((i &= 0x0f) != 0) {
        if (i < 8) printf(" ");
        for (; i<16; i++) {
            printf("   ");
            str[i] = ' ';
        }
        printf(" |%16s|", str);
    }
    printf("\n");
}

