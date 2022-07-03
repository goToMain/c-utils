/*
 * Copyright (c) 2020-2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _UTILS_UTILS_H_
#define _UTILS_UTILS_H_

#include <stdint.h>
#include <stddef.h>

#ifndef NULL
#define NULL                           ((void *)0)
#endif

#ifndef TRUE
#define TRUE                           (1)
#endif

#ifndef FALSE
#define FALSE                          (0)
#endif

#define BYTE_0(x)                      (uint8_t)(((x) >>  0) & 0xFF)
#define BYTE_1(x)                      (uint8_t)(((x) >>  8) & 0xFF)
#define BYTE_2(x)                      (uint8_t)(((x) >> 16) & 0xFF)
#define BYTE_3(x)                      (uint8_t)(((x) >> 24) & 0xFF)

#define BIT(n)                         (1ull << (n))
#define MASK(n)                        (BIT((n) + 1) - 1)
#define BIT_IS_SET(m, n)               (bool)((m) & BIT(n))
#define BIT_SET(m, n)                  ((m) |=  BIT(n))
#define BIT_CLEAR(m, n)                ((m) &= ~BIT(n))
#define BIT_FLIP(m, n)                 ((m) ^=  BIT(n))
#define BIT_TEST_SET(m, n)             ({ BIT_IS_SET(m, n) ? false : (bool) BIT_SET(m, n) })

#define ARG_UNUSED(x)                  (void)(x)

#define STR(x) #x
#define XSTR(x) STR(x)

#define ROUND_UP(x, y) ((x + y - 1) & ~ (y - 1))

#define MATH_MOD(a, b)                 (((a % b) + b) % b)

#define IS_POW2(n)                     ((n) & ((n) - 1))

#define ARRAY_SIZEOF(x) \
	(sizeof(x) / sizeof(x[0]))

#define ARRAY_BASE(ptr, type, offset) \
	((char *)(ptr)) - ((sizeof(type)) * offset)

#define OFFSET_OF(type, field) (size_t)(&((type *)(0))->field)

#define CONTAINER_OF(ptr, type, field) \
        ((type *)(((char *)(ptr)) - OFFSET_OF(type, field)))

#define MAX(a,b) ({ \
		__typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _a : _b; \
	})

#define MIN(a,b) ({ \
		__typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _b : _a; \
	})

#define SWAP(a,b) { \
		__typeof__ (a) _tmp; \
		_tmp = a; \
		b = a; \
		a = _tmp; \
	}

#define __READ_ONCE(x)	(*(volatile typeof(x) *)&(x))
#define READ_ONCE(x) ({ \
		typeof(x) *__xp = &(x); \
		typeof(x) __x = __READ_ONCE(*__xp); \
		__x; \
	})

#define WRITE_ONCE(x, val) do { \
		typeof(x) *__xp = &(x); \
		*(volatile typeof(x) *)__xp = (val); \
	} while (0)

#define ABS(x)		((x) >= 0  ? (x) : -(x))

/* config_enabled() from the kernel */
#define __IS_ENABLED1(x)             __IS_ENABLED2(__XXXX ## x)
#define __XXXX1                       __YYYY,
#define __IS_ENABLED2(y)             __IS_ENABLED3(y 1, 0)
#define __IS_ENABLED3(_i, val, ...)   val

#define IS_ENABLED(x)                 __IS_ENABLED1(x)

/* gcc attribute shorthands */
#ifndef __fallthrough
#if __GNUC__ >= 7
#define __fallthrough        __attribute__((fallthrough))
#else
#define __fallthrough
#endif
#endif

#ifndef __packed
#define __packed        __attribute__((__packed__))
#endif

#undef __weak
#define __weak          __attribute__((weak))

/**
 * @brief Return random number between `min` and `max` both inclusive.
 *
 * Note: the random number generator must be pre-seeded.
 */
int randint(int min, int max);

/**
 * @brief Rounds up 32-bit v to nearest power of 2. If v is already a power
 * of 2 it is returned unmodified.
 */
uint32_t round_up_pow2(uint32_t v);

/**
 * @brief Retruns number of digits in a given number in its decimal form.
 */
int num_digits_in_number(int num);

/**
 * @brief Dumps an array of bytes in HEX and ASCII formats for debugging. `head`
 * is string that is printed before the actual bytes are dumped.
 *
 * Example:
 * 	int len;
 * 	uint8_t data[MAX_LEN];
 * 	len = get_data_from_somewhere(data, MAX_LEN);
 * 	hexdump(data, len, "Data From Somewhere");
 */
void hexdump(const void *data, size_t len, const char *fmt, ...);

/**
 * @brief Get the time in micro seconds.
 */
int64_t usec_now();

/**
 * @brief Get time elapsed in micro seconds since `last`. Used along with
 * usec_now().
 */
int64_t usec_since(int64_t last);

/**
 * @brief Get the time in milli seconds.
 */
int64_t millis_now();

/**
 * @brief Get time elapsed in milli seconds since `last`. Used along with
 * millis_now().
 */
int64_t millis_since(int64_t last);

void dump_trace(void);

#endif /* _UTILS_UTILS_H_ */
