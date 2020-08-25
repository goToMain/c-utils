/*
 * Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <utils/utils.h>

static void die_oom(const char *msg, size_t count, size_t size)
{
	fprintf(stderr, "fatal: %s() out of memory during alloc for %zu*%zu\n",
		msg, count, size);
	exit(-1);
}

void safe_free(void *p)
{
	if (p != NULL) {
		free(p);
	}
}

void *safe_malloc(size_t size)

{
	void *p;

	p = malloc(size);

	if (p == NULL)
		die_oom("malloc", 1, size);

	return p;
}

void *safe_calloc(size_t count, size_t size)
{
	void *p;

	p = calloc(count, size);

	if (p == NULL)
		die_oom("calloc", count, size);

	return p;
}

void *safe_strdup(const char *s)
{
	char *p;

	p = strdup(s);

	if (p == NULL)
		die_oom("strdup", 1, strlen(s));

	return p;
}

void *safe_realloc(void *data, size_t size)
{
	void *p;

	p = realloc(data, size);
	if (p == NULL)
		die_oom("realloc", 1, size);

	return p;
}

void *safe_realloc_zero(void *data, size_t old_size, size_t new_size)
{
	void *p;

	assert(old_size != new_size);

	p = safe_realloc(data, new_size);
	if (new_size > old_size) {
		memset((unsigned char *)p + old_size, 0, new_size - old_size);
	}
	return p;
}

uint32_t round_up_pow2(uint32_t v)
{
	/* from the bit-twiddling hacks */
	v -= 1;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v += 1;
	return v;
}

void hexdump(const char *head, const uint8_t *data, size_t len)
{
	size_t i;
	char str[16 + 1] = {0};

	printf("%s [%zu] =>\n    0000  %02x ", head, len, data[0]);
	str[0] = isprint(data[0]) ? data[0] : '.';
	for (i = 1; i < len; i++) {
		if ((i & 0x0f) == 0) {
			printf(" |%16s|", str);
			printf("\n    %04zu  ", i);
		} else if((i & 0x07) == 0) {
			printf(" ");
		}
		printf("%02x ", data[i]);
		str[i & 0x0f] = isprint(data[i]) ? data[i] : '.';
	}
	if ((i &= 0x0f) != 0) {
		if (i <= 8) printf(" ");
		while (i < 16) {
			printf("   ");
			str[i++] = ' ';
		}
		printf(" |%16s|", str);
	}
	printf("\n");
}
