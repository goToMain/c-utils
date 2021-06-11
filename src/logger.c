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
	YEL,   MAG,   GRN,   GRN
};

static const char *log_level_names[LOG_MAX_LEVEL] = {
	"EMERG", "ALERT", "CRIT ", "ERROR",
	"WARN ", "NOTIC", "INFO ", "DEBUG"
};

static void logger_log_set_color(logger_t *ctx, const char *color)
{
	int ret, len;

	len = strnlen(color, 8);
	if (isatty(fileno(ctx->file))) {
		ret = write(fileno(ctx->file), color, len);
		assert(ret == len);
		ARG_UNUSED(ret); /* squash warning in Release builds */
	}
}

int logger_log(logger_t *ctx, int log_level, const char *tag, const char *fmt, ...)
{
	int ret;
	va_list args;

	if (ctx->file == NULL)
		ctx->file = stderr;

	if (log_level < LOG_EMERG ||
	    log_level >= LOG_MAX_LEVEL ||
	    log_level > ctx->log_level)
		return 0;

	logger_log_set_color(ctx, log_level_colors[log_level]);

	va_start(args, fmt);
	ret = fprintf(ctx->file, "%s: %s: %s: ", ctx->module, tag,
				log_level_names[log_level]);
	ret += vfprintf(ctx->file, fmt, args);
	ret += fprintf(ctx->file, "\n");
	va_end(args);

	logger_log_set_color(ctx, RESET);
	return ret;
}
