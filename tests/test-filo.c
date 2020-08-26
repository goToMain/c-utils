#include "test.h"

#include <utils/filo.h>
#include <utils/utils.h>

#include <string.h>

#define NUM_TESTS               100

FILO_DEF(filo_stack, int, NUM_TESTS);

void do_test_filo(test_t *ctx, test_result_t *result)
{
    int i;
	int total=0, pass=0;

    for (i = 0; i < NUM_TESTS; i++) {
        int elem = i + 1;
        if (FILO_PUSH(filo_stack, &elem) == 0)
            pass++;
        else
            break;
        total++;
    }

    if (total != pass)
        goto exit;

    for (i = 0; i < NUM_TESTS; i++) {
        int elem = 0;
        if ((FILO_POP(filo_stack, &elem) == 0) && (elem == (NUM_TESTS - i)))
            pass++;
        else
            break;
        total++;
    }

exit:
	result->pass = pass;
	result->total = total;
}
