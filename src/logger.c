/*
 * Copyright (c) 2020-2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define _GNU_SOURCE		/* See feature_test_macros(7) */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <utils/logger.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

static const char *log_level_colors[LOG_MAX_LEVEL] = {
	RED,   RED,   RED,   RED,
	YEL,   MAG,   GRN,   RESET
};

static const char *log_level_names[LOG_MAX_LEVEL] = {
	"EMERG", "ALERT", "CRIT ", "ERROR",
	"WARN ", "NOTIC", "INFO ", "DEBUG"
};

void logger_clear_prefix(logger_t *ctx)
{
	if (ctx->prefix && (ctx->flags & LOGGER_FLAG_PREFIX_ALLOC))
		free(ctx->prefix);
	ctx->prefix = NULL;
	ctx->flags &= ~(LOGGER_FLAG_PREFIX_ALLOC | LOGGER_FLAG_HAS_PREFIX);
}

void logger_set_prefix(logger_t *ctx, const char *fmt, ...)
{
	int ret = 0;
	va_list args;

	logger_clear_prefix(ctx);
	va_start(args, fmt);
	ret = vasprintf(&ctx->prefix, fmt, args);
	assert(ret > 0);
	ARG_UNUSED(ret); /* squash warning in Release builds */
	va_end(args);
	ctx->flags |= LOGGER_FLAG_HAS_PREFIX;
	ctx->flags |= LOGGER_FLAG_PREFIX_ALLOC;
}

void logger_set_prefix_const(logger_t *ctx, const char *prefix)
{
	logger_clear_prefix(ctx);
	ctx->prefix = (char *)prefix;
	ctx->flags |= LOGGER_FLAG_HAS_PREFIX;
}

void logger_set_log_level(logger_t *ctx, int log_level)
{
	ctx->log_level = log_level;
}

void logger_set_put_fn(logger_t *ctx, log_puts_fn_t fn)
{
	ctx->puts = fn;
}

void logger_set_file(logger_t *ctx, FILE *f)
{
	ctx->file = f;
}

static inline void logger_log_set_color(logger_t *ctx, const char *color)
{
	int ret, len;

	if (ctx->flags & LOGGER_FLAG_NO_COLORS)
		return;

	if (ctx->file && isatty(fileno(ctx->file))) {
		len = strnlen(color, 8);
		ret = write(fileno(ctx->file), color, len);
		assert(ret == len);
		ARG_UNUSED(ret); /* squash warning in Release builds */
	}
}

#define LOG_BUF_LEN 128

__attribute__((format(printf, 4, 5)))
int logger_log(logger_t *ctx, int log_level, const char *tag,
	       const char *fmt, ...)
{
	int len = 0;
	va_list args;
	char buf[LOG_BUF_LEN + 2]; /* +2 for tailing '\n' and '\0' */

	assert(ctx);
	assert(ctx->file || ctx->puts);

	if (log_level < LOG_EMERG ||
	    log_level >= LOG_MAX_LEVEL ||
	    log_level > ctx->log_level)
		return 0;

	/* print module and log_level prefix */
	len = snprintf(buf, LOG_BUF_LEN, "%s: %s: %s: ", ctx->module, tag,
		       log_level_names[log_level]);
	if (len > LOG_BUF_LEN)
		goto out;

	/* Any other prefix set by user */
	if (ctx->flags & LOGGER_FLAG_HAS_PREFIX) {
		len += snprintf(buf + len, LOG_BUF_LEN - len, "%s: ", ctx->prefix);
		if (len > LOG_BUF_LEN)
			goto out;
	}

	/* Print the actual message */
	va_start(args, fmt);
	len += vsnprintf(buf + len, LOG_BUF_LEN - len, fmt, args);
	va_end(args);
	if (len > LOG_BUF_LEN)
		goto out;

out:
	if (len > LOG_BUF_LEN)
		len = LOG_BUF_LEN;

	/* If the log line doesn't already end with a new line, add it */
	if(buf[len - 1] != '\n')
		buf[len++] = '\n';
	buf[len] = '\0';

	logger_log_set_color(ctx, log_level_colors[log_level]);

	if (ctx->file)
		fputs(buf, ctx->file);
	else
		ctx->puts(buf);

	logger_log_set_color(ctx, RESET);

	return len;
}
