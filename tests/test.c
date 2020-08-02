/*
 * Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#include "test.h"

#define DATE_TIME_STR_LEN 64

char date_time[DATE_TIME_STR_LEN];

void do_test_circular_buffer(test_result_t *result);
void do_test_strlib(test_result_t *result);

test_module_t c_utils_test_modules[] = {
    {"Circular Buffers", do_test_circular_buffer },
    {"String Library", do_test_strlib },
    {NULL, NULL}
};

void test_execute(test_t *t, test_module_t *tm)
{
    test_result_t result;

    printf("Testing '%s'\n", tm->name);
    tm->runner(&result);
    mod_printf("Result: name:'%s' total:%d pass:%d\n",
               tm->name, result.total, result.pass);
    t->pass += result.pass;
    t->total += result.total;
}

char *time_string(time_t *t)
{
    strftime(date_time, DATE_TIME_STR_LEN, "%d/%m/%y-%H:%M:%S %z", localtime(t));
    return date_time;
}

int main(int argc, char *argv[])
{
    test_module_t *tm;
    test_t test;

    // Test header
    printf("\n");
    printf("------------------------------------------\n");
    printf("    EmbedJournal C Unit Test Framework    \n");
    printf("------------------------------------------\n");

    // Tests
    memset(&test, 0, sizeof(test_t));
    test.start_time = time(NULL);
    tm = &(c_utils_test_modules[0]);
    while (tm->name != NULL) {
        test_execute(&test, tm);
        tm++;
    }
    test.end_time = time(NULL);

    // Test summary
    printf("------------------------------------------\n");
    printf("                Test Summary              \n");
    printf("------------------------------------------\n");
    printf("Start Time: %s\n", time_string(&test.start_time));
    printf("End Time:   %s\n", time_string(&test.end_time));
    printf("Executed: %d\n", test.total);
    printf("Sucessful: %d\n", test.pass);
    printf("Resut: %s\n\n", (test.total == test.pass) ? "PASS" : "FAIL");
    return 0;
}
