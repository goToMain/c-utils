/*
 * Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _TEST_H_
#define _TEST_H_

#include <time.h>
#include <stdio.h>

#define mod_printf(fmt, ...) fprintf(stdout,"  - "fmt"\n", ## __VA_ARGS__)

typedef struct
{
    int total;
    int pass;
} test_result_t;

typedef struct {
    const char *name;
    void (*runner)(test_result_t *result);
} test_module_t;

typedef struct {
    time_t start_time;
    time_t end_time;
    int total;
    int pass;
} test_t;

#endif
