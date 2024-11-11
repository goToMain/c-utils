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

__format_printf(3, 4)
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

#if (defined(_WIN32) || defined(_WIN64))
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h> // portable: uint64_t   MSVC: __int64

// MSVC defines this in winsock2.h!?
typedef struct timeval {
	long tv_sec;
	long tv_usec;
} timeval;

int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
	// Note: some broken versions only have 8 trailing zero's, the correct
	// epoch has 9 trailing zero's This magic number is the number of 100
	// nanosecond intervals since January 1, 1601 (UTC) until 00:00:00
	// January 1, 1970
	static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

	SYSTEMTIME  system_time;
	FILETIME    file_time;
	uint64_t    time;

	GetSystemTime( &system_time );
	SystemTimeToFileTime( &system_time, &file_time );
	time =  ((uint64_t)file_time.dwLowDateTime )      ;
	time += ((uint64_t)file_time.dwHighDateTime) << 32;

	tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
	tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
	return 0;
}

int add_iso8601_utc_datetime(char* buf, size_t size)
{
	int r;
	SYSTEMTIME utcTime;
	GetSystemTime(&utcTime); // Get the current UTC time

	// Format: YYYY-MM-DDThh:mm:ssZ
	r = snprintf(buf, size, "%04d-%02d-%02dT%02d:%02d:%02dZ",
		     utcTime.wYear, utcTime.wMonth, utcTime.wDay,
		     utcTime.wHour, utcTime.wMinute, utcTime.wSecond);
	return r;
}

#elif defined(__linux__) || defined(__APPLE__)

#include <sys/time.h>
#include <time.h>

int add_iso8601_utc_datetime(char *buf, size_t size)
{
	time_t now;
	struct tm timeinfo;

	// Format: YYYY-MM-DDThh:mm:ssZ
	time(&now);
	gmtime_r(&now, &timeinfo);

	return strftime(buf, size, "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
}

#elif defined(__BARE_METAL__)

#ifndef _TIMEVAL_DEFINED
struct timeval {
	long tv_sec;  // seconds since epoch
	long tv_usec; // microseconds
};
#endif

#ifndef _TIMEZONE_DEFINED
struct timezone {
	int tz_minuteswest; // minutes west of UTC
	int tz_dsttime;     // daylight saving time flag
};
#endif

int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
	ARG_UNUSED(tzp);
	tp->tv_sec = 0;
	tp->tv_usec = 0;
	return 0;
}

int add_iso8601_utc_datetime(char* buf, size_t size) {
	ARG_UNUSED(buf);
	ARG_UNUSED(size);
	return 0;
}

#else

#error Platform test failed

#endif

int64_t usec_now()
{
	int64_t usec;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	usec = tv.tv_sec * 1000LL * 1000LL + tv.tv_usec;

	return usec;
}

void get_time(uint32_t *seconds, uint32_t *micro_seconds)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	*seconds = tv.tv_sec;
	*micro_seconds = tv.tv_usec;
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
