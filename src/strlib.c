/*
 * Copyright (c) 2019 EmbedJournal
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "strlib.h"

int str_printf(string_t *s, const char *mode, const char *fmt, ...)
{
	int ret, start;
	va_list (args);

	start = (mode[0] == 'a') ? s->len : 0;
	va_start(args, fmt);
	ret = vsnprintf(s->buf + start, s->max_len - start, fmt, args);
	va_end(args);

	if ((start + ret) >= s->max_len)
		return -1;

	s->len = start + ret;
	s->buf[s->len] = 0;
	return ret;
}

int str_copy(string_t *s, const char *mode, const char *str, size_t len)
{
	int start;

	start = (mode[0] == 'a') ? s->len : 0;
	if ((start + len) > s->max_len) {
		if (mode[1] != 'f') {
			return -1;
		}
		len = s->max_len - start;
	}
	memcpy(s->buf + start, str, len);
	s->len = start + len;
	s->buf[s->len] = 0;
	return len;
}
