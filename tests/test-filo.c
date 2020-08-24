#include "test.h"

#include <utils/filo.h>
#include <utils/utils.h>

#include <string.h>

#define NUM_TESTS               100

static int test_push(filo_t *pfilo, int elem)
{
    return filo_push(pfilo, &elem);
}

static int test_pop(filo_t *pfilo, int *pelem)
{
	return filo_pop(pfilo, pelem, true);
}

void do_test_filo(test_t *ctx, test_result_t *result)
{
	filo_t *pfilo = filo_alloc(sizeof(int), NUM_TESTS);
    int i;
	int total=0, pass=0;

    for (i = 0; i < NUM_TESTS; i++) {
        if (test_push(pfilo, i + 1) == 0)
            pass++;
        else
            break;
        total++;
    }

    if (total != pass)
        goto EXIT;

    for (i = 0; i < NUM_TESTS; i++) {
        int element = 0;
        if ((test_pop(pfilo, &element) == 0) && (element == (NUM_TESTS - i)))
            pass++;
        else
            break;
        total++;
    }

EXIT:
	filo_free(pfilo);
	result->pass = pass;
	result->total = total;
}
