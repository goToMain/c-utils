/*
 * Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <utils/circbuf.h>

#include "test.h"

struct test_struct
{
	int a;
	char b;
};

CIRC_GBUF_DEF(struct test_struct, test_cb, 10);

int test_boundary()
{
	int i, iter = 0, next_write = 0, next_read = 0;
	struct test_struct s;

	CIRC_GBUF_FLUSH(test_cb);

	do {
		for (i = 0; i < 3; i++) {
			s.a = next_write++;
			if (CIRC_GBUF_PUSH(test_cb, &s)) {
				mod_printf("push failed at iter:%d", iter);
				return -1;
			}
		}
		for (i = 0; i < 3; i++) {
			if (CIRC_GBUF_POP(test_cb, &s)) {
				mod_printf("pop failed at iter:%d", iter);
				return -1;
			}
			if (next_read != s.a) {
				mod_printf("invalid data at iter:%d read:%d expected:%d",
					   iter, s.a, next_read);
				return -1;
			}
			next_read++;
		}
		iter++;
	} while (iter < 100);

	return 0;
}

#define LIMIT 1000
#define W_PROB 1.0
#define R_PROB 0.01

double r2()
{
	return (double)rand() / (double)RAND_MAX;
}

int test_probabilistic()
{
	struct test_struct m;
	int next_write = 1;
	int expected_read = 1;
	int done_writing = 0;
	int done_reading = 0;

	CIRC_GBUF_FLUSH(test_cb);
	srand(time(NULL));

	while (1)
	{
		if ((r2() < W_PROB) && !done_writing) {
			m.a = next_write;
			if (CIRC_GBUF_PUSH(test_cb, &m) == 0) {
				next_write++;
				if (next_write > LIMIT) {
					done_writing = 1;
				}
			}
		}
		if (r2() < R_PROB) {
			if (CIRC_GBUF_POP(test_cb, &m) == 0) {
				if (m.a != expected_read) {
					mod_printf("invalid data, got %d, expected %d",
						   m.a, expected_read);
					return -1;
				}
				if (m.a >= LIMIT) {
					done_reading = 1;
				}
				expected_read++;
			}
		}
		if (done_reading && done_writing)
			break;
	}
	return 0;
}

TEST_DEF(circular_buffer)
{
	TEST_MOD_INIT();

	TEST_MOD_EXEC(test_boundary());
	TEST_MOD_EXEC(test_probabilistic());

	TEST_MOD_REPORT();
}
