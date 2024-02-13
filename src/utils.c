/*
 * Copyright (c) 2020-2024 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/time.h>

#include <utils/utils.h>

int randint(int limit)
{
	int r;
	int divisor = RAND_MAX / (limit + 1);

	do {
		r = rand() / divisor;
	} while (r > limit);
	return r;
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

int num_digits_in_number(int num)
{
	int digits = 0, n = ABS(num);

	while (n > 0) {
		digits++;
		n /= 10;
	}
	return digits;
}

__attribute__((format(printf, 3, 4)))
void hexdump(const void *p, size_t len, const char *fmt, ...)
{
	size_t i;
	va_list args;
	char str[16 + 1] = {0};
	const uint8_t *data = p;

	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);

	printf(" [%zu] =>\n    0000  %02x ", len, data[0]);
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
	} else {
		printf(" |%16s|", str);
	}

	printf("\n");
}

int64_t usec_now()
{
	int64_t usec;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	usec = tv.tv_sec * 1000LL * 1000LL + tv.tv_usec;

	return usec;
}

int64_t usec_since(int64_t last)
{
	return usec_now() - last;
}

int64_t millis_now()
{
	return (int64_t)(usec_now() / 1000LL);
}

int64_t millis_since(int64_t last)
{
	return millis_now() - last;
}

#if (defined(__linux__) || defined(__APPLE__)) && defined(__GLIBC__)
#include <execinfo.h>
void dump_trace(void)
{
	char **strings;
	size_t i, size;
	void *array[1024];

	size = backtrace(array, sizeof(array));
	strings = backtrace_symbols(array, size);
	for (i = 0; i < size; i++) {
		printf("\t%s\n", strings[i]);
	}
	puts("");
	free(strings);
}
#else
void dump_trace(void)
{
	puts("Stack trace unavailable!\n");
}
#endif
