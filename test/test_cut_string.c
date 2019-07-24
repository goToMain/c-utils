#include "test.h"
#include "string.h"
#include "cut_string.h"

void do_test_cut_string(test_result_t *result)
{
    int total=0, pass=0;
    char dest[32], *p;
    char *src = "hello,world,test,prog";
    
    p = cut_string(src, dest, 32, ',', 1);

    if (strcmp(dest, "world") == 0) pass++; total++;
    if (strcmp(p, ",test,prog") == 0) pass++; total++;

    result->pass = pass;
    result->total = total;
}
