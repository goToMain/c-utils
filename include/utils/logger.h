/*
 * Copyright (c) 2020-2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>
#include <utils/utils.h>

/**
 * @brief A printf() like method that will be used to write out log lines.
 *
 * @param fmt C printf() style format string. See man 3 printf
 *
 * @retval number of characters written to the log stream
 */
typedef int (*log_puts_fn_t)(const char *buf);

typedef struct {
	const char *module;
	int log_level;
	uint32_t flags;
	FILE *file;
	log_puts_fn_t puts;
	char *prefix;
} logger_t;

enum log_levels {
	LOG_EMERG,
	LOG_ALERT,
	LOG_CRIT,
	LOG_ERR,
	LOG_WARNING,
	LOG_NOTICE,
	LOG_INFO,
	LOG_DEBUG,
	LOG_MAX_LEVEL
};

#define LOGGER_FLAG_NONE                   0x00000000
#define LOGGER_FLAG_NO_COLORS              0x00000001
#define LOGGER_FLAG_HAS_PREFIX             0x00000002
#define LOGGER_FLAG_PREFIX_ALLOC           0x00000004

int logger_log(logger_t *ctx, int log_level, const char *tag, const char *fmt, ...);
void logger_set_log_level(logger_t *ctx, int log_level);
void logger_set_put_fn(logger_t *ctx, log_puts_fn_t fn);
void logger_set_file(logger_t *ctx, FILE *f);
void logger_clear_prefix(logger_t *ctx);
void logger_set_prefix(logger_t *ctx, const char *prefix);

#define LOGGER_DEFINE(mod_name, lvl, fl) \
	logger_t mod_name ## _logger_ctx = { \
		.module = STR(mod_name), \
		.log_level = lvl, \
		.flags = fl, \
		.file = NULL, \
		.puts = NULL, \
	}; \
	logger_t *get_ ## mod_name ## _logger_ctx() { \
		return &mod_name ## _logger_ctx; \
	};

#define get_log_ctx(mod_name) get_ ## mod_name ## _logger_ctx()

#define LOGGER_DECLARE(mod_name, tag) \
	static const char * const LOG_TAG __attribute__((unused)) = tag; \
	extern logger_t *get_ ## mod_name ## _logger_ctx(); \
	static inline logger_t *get_current_logger_ctx() { \
		return get_ ## mod_name ## _logger_ctx(); \
	}

#define LOG_EM(...)    logger_log(get_current_logger_ctx(), LOG_EMERG,  LOG_TAG, __VA_ARGS__)
#define LOG_ALERT(...) logger_log(get_current_logger_ctx(), LOG_ALERT,  LOG_TAG, __VA_ARGS__)
#define LOG_CRIT(...)  logger_log(get_current_logger_ctx(), LOG_CRIT,   LOG_TAG, __VA_ARGS__)
#define LOG_ERR(...)   logger_log(get_current_logger_ctx(), LOG_ERR,    LOG_TAG, __VA_ARGS__)
#define LOG_INF(...)   logger_log(get_current_logger_ctx(), LOG_INFO,   LOG_TAG, __VA_ARGS__)
#define LOG_WRN(...)   logger_log(get_current_logger_ctx(), LOG_WARNING,LOG_TAG, __VA_ARGS__)
#define LOG_NOT(...)   logger_log(get_current_logger_ctx(), LOG_NOTICE, LOG_TAG, __VA_ARGS__)
#define LOG_DBG(...)   logger_log(get_current_logger_ctx(), LOG_DEBUG,  LOG_TAG, __VA_ARGS__)
#define LOG_PRINT(...) logger_log(get_current_logger_ctx(), LOG_INFO,   LOG_TAG, __VA_ARGS__)

#define LOG_SET_PREFIX(str) logger_set_prefix(get_current_logger_ctx(), str);
#define LOG_CLEAR_PREFIX() logger_clear_prefix(get_current_logger_ctx())

#endif /* _LOGGER_H_ */
