/*
 * Copyright (c) 2019 EmbedJournal
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef _UTIL_STRLIB_H_
#define _UTIL_STRLIB_H_

#include <stddef.h>

typedef struct {
	char *buf;
	size_t len;
	size_t max_len;
} string_t;

/**
 * Description:
 * 	Defines a local instance string_t of specified length.
 *
 * Example:
 * 	LOCAL_STRING_DEF(my_string, 100);
 */
#define LOCAL_STRING_DEF(str, length)	\
	string_t str;			\
	char str ## _buf[length+1]={0}; \
	str.buf = str ## _buf;		\
	str.len = 0;			\
	str.max_len = length;

/**
 * Description:
 * 	Defines a global instance string_t of specified length.
 *
 * Example:
 * 	GLOBAL_STRING_DEF(my_string, 100);
 */
#define GLOBAL_STRING_DEF(str, length)	\
	char str ## _buf[length+1]={0};	\
	string_t str = {		\
		.buf = str ## _buf,	\
		.len = 0,		\
		.max_len = length	\
	}

/**
 * String Ops Modes:
 *
 *  - "a" (append): bytes are appended to the existing contents of
 *    the string. If there is not enough space in the string, an
 *    error is returned.
 *
 *  - "af" (append, fill): same as "a" but fills as many bytes as
 *     possible into the string.
 *
 *  - "c" (create/copy): bytes are copied to the beginning of the
 *    string. If there is not enough space in the string, an
 *    error is returned.
 *
 *  - "cf" (copy, fill): same as "c" but fills as many bytes as
 *    possible into the string.
 */

/**
 * Description:
 * 	Do formated print operations (as with printf) on string_t.
 *
 * Note:
 * 	Use `mode` as defined above in "String Ops Modes" section.
 *
 * Example:
 * 	str_printf(&my_string, "c", "Age: %d\n", 28);
 * 	// now, my_string == "Age: 28\n"
 */
int str_printf(string_t *s, const char *mode, const char *fmt, ...);

/**
 * Description:
 * 	Copy contents of `buf` of length `len` into string `s`.
 *
 * Note:
 * 	Use `mode` as defined above in "String Ops Modes" section.
 *
 * Example:
 * 	str_copyf(&my_string, "c", "Hello", 5);
 * 	// now, my_string == "Hello"
 * 	str_copy(&my_string, "a", " World", 6);
 * 	// now, my_string == "Hello World"
 */
int str_copy(string_t *s, const char *mode, const char *str, size_t len);

/* macro helpers */

#define str_flush(s)			\
	do {				\
		(s)->buf[0] = 0;	\
		(s)->len = 0;		\
	} while(0)
#define str_const_copy(s, str)		str_copy(s, "c", str, strlen(s))
#define str_const_append(s, str)	str_copy(s, "a", str, strlen(str))

#endif /* _UTIL_STRLIB_H_ */
