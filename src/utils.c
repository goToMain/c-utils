/*
 * Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

void safe_free(void *p)
{
	if (p != NULL) {
		free(p);
	}
}

void *safe_zalloc(size_t size)
{
	void *p;

	p = calloc(1, size);
	if (p == NULL) {
		printf("fatal: alloc failed! Exiting..\n");
		exit(-1);
	}
	return p;
}
