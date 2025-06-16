/*
 * Copyright (c) 2020-2024 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _UTILS_UTILS_H_
#define _UTILS_UTILS_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

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

#ifndef BIT
#define BIT(n)                         (1ull << (n))
#endif
#define MASK(n)                        (BIT((n) + 1) - 1)
#define BIT_IS_SET(m, n)               (bool)((m) & BIT(n))
#define BIT_SET(m, n)                  ((m) |=  BIT(n))
#define BIT_CLEAR(m, n)                ((m) &= ~BIT(n))
#define BIT_FLIP(m, n)                 ((m) ^=  BIT(n))
#define BIT_TEST_SET(m, n)             ({ BIT_IS_SET(m, n) ? false : (bool) BIT_SET(m, n) })

#define ARG_UNUSED(x)                  (void)(x)

#define STR(x) #x
#define XSTR(x) STR(x)

#ifndef __ZEPHYR__
#define STRINGIFY(x) #x
#endif

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

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

#ifdef __weak
#undef __weak
#endif

#if (defined(_WIN32) || defined(_WIN64))

#include <io.h>
#define isatty                  _isatty /* from io.h */
#define fileno                  _fileno
#define __format_printf(x, y)
#define __noreturn              __declspec(noreturn)
#define __weak
#define __unreachable()         __assume(0)
#define likely(p)               (p)
#define unlikely(p)             (p)
#define PATH_SEPARATOR          '\\'

#elif defined(__ZEPHYR__)

#include <zephyr/toolchain.h>  // __weak, __builtin_xxx() etc.,

#define PATH_SEPARATOR          '/'
#define __unreachable()         __builtin_unreachable()
#define __format_printf(x, y)   __attribute__((format(printf, x, y)))

#else

#define __format_printf(x, y)   __attribute__((format(printf, x, y)))
#define __noreturn              __attribute__((noreturn))
#define __weak                  __attribute__((weak))
#define __unreachable()         __builtin_unreachable()
#define likely(p)               __builtin_expect(!!(p), 1)
#define unlikely(p)             __builtin_expect(!!(p), 0)
#define PATH_SEPARATOR          '/'
#endif

/**
 * @brief Return random number between 0 and `limit` both inclusive.
 *
 * Note: the random number generator must be pre-seeded.
 */
int randint(int limit);

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
 * @brief Get time in seconds and micro_seconds
 */
void get_time(uint32_t *seconds, uint32_t *micro_seconds);

/**
 * @brief Addd an ISO 8601 datetime string to `buf` for upto `size` bytes.
 *
 * Note: The format is YYYY-MM-DDThh:mm:ssZ, so size must alteast be 21.
 */
int add_iso8601_utc_datetime(char *buf, size_t size);

/**
 * @brief Get time elapsed in milli seconds since `last`. Used along with
 * millis_now().
 */
int64_t millis_since(int64_t last);

/**
 * @brief Print the stack trace
 */
void dump_trace(void);

static inline bool char_is_space(int c)
{
	unsigned char d = (unsigned char)(c - 9);
	return (0x80001FU >> (d & 31)) & (1U >> (d >> 5));
}

static inline bool char_is_digit(int c)
{
	return (unsigned int)(('0' - 1 - c) & (c - ('9' + 1))) >> (sizeof(c) * 8 - 1);
}

static inline bool char_is_alpha(int c)
{
	return (unsigned int)(('a' - 1 - (c | 32)) & ((c | 32) - ('z' + 1))) >> (sizeof(c) * 8 - 1);
}

inline uint8_t u8_bit_reverse(uint8_t b)
{
	b = (((b & 0xaa) >> 1) | ((b & 0x55) << 1));
	b = (((b & 0xcc) >> 2) | ((b & 0x33) << 2));
	return ((b >> 4) |  (b << 4));
}

inline uint16_t u16_bit_reverse(uint16_t x)
{
	x = (((x & 0xaaaa) >> 1) | ((x & 0x5555) << 1));
	x = (((x & 0xcccc) >> 2) | ((x & 0x3333) << 2));
	x = (((x & 0xf0f0) >> 4) | ((x & 0x0f0f) << 4));
	return((x >> 8) | (x << 8));
}

inline uint32_t u32_bit_reverse(uint32_t x)
{
	x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
	x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
	x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
	x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
	return ((x >> 16) | (x << 16));
}

#ifdef __cplusplus
}
#endif

#endif /* _UTILS_UTILS_H_ */
