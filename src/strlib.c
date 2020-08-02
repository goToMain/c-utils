/*
 * Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include <utils/strlib.h>
#include <utils/utils.h>

void string_create(string_t *s, const char *buf, size_t len)
{
	s->buf = safe_zalloc(sizeof(char) * (len + 1));
	s->len = 0;
	s->max_len = len;
	if (buf != NULL) {
		memcpy(s->buf, buf, len);
		s->len = len;
	}
	s->buf[len] = '\0'; /* Make sure there is always a terminating null */
}

int string_resize(string_t *s, size_t new_len)
{
	char *temp;

	temp = realloc(s->buf, new_len + 1);
	if (temp == NULL) {
		return -1;
	}
	s->buf = temp;
	s->max_len = new_len;
	if (s->len > new_len) {
		s->len = new_len;
	}
	s->buf[new_len] = '\0';
	return 0;
}

void string_destroy(string_t *s)
{
	safe_free(s->buf);
	s->buf = NULL;
	s->len = 0;
	s->max_len = 0;
}

int string_printf(string_t *s, const char *mode, const char *fmt, ...)
{
	int ret, start;
	va_list (args);

	start = (mode[0] == 'a') ? s->len : 0;
	va_start(args, fmt);
	ret = vsnprintf(s->buf + start, s->max_len - start, fmt, args);
	va_end(args);

	if ((start + ret) >= (int)s->max_len) {
		return -1;
	}

	s->len = start + ret;
	s->buf[s->len] = 0;
	return ret;
}

int string_copy(string_t *s, const char *mode, const char *str, size_t len)
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
	s->buf[s->len] = '\0';
	return 0;
}

int string_merge(string_t *primary, string_t *secondary)
{
	size_t target;

	target = primary->len + secondary->len;
	if (target > primary->max_len) {
		if (string_resize(primary, target) != 0) {
			return -1;
		}
	}
	string_copy(primary, "a", secondary->buf, secondary->len);
	string_destroy(secondary);
	return 0;
}
