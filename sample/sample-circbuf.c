#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "circbuf.h"

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
