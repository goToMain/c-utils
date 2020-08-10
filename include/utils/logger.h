/*
 * Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>
#include <utils/utils.h>

typedef struct {
	const char *module;
	int log_level;
	FILE *file;
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

int logger_log(logger_t *ctx, int log_level, const char *tag, const char *fmt, ...);

#define LOGGER_MODULE_DEFINE(mod_name, lvl)      \
	logger_t mod_name ## _logger_ctx = {     \
		.module = STR(mod_name),         \
		.log_level = lvl,                \
		.file = NULL,                    \
	};                                       \
	static const char *LOG_TAG = "main";     \
	logger_t *get_logger_ctx() {             \
		return &mod_name ## _logger_ctx; \
	}

#define LOGGER_MODULE_EXTERN(n, t)            \
	static const char *LOG_TAG = STR(t);  \
	extern logger_t *get_logger_ctx();

#define LOGGER_MODULE_SET(mod_name, lvl, f)	  \
	mod_name ## _logger_ctx.log_level = lvl;  \
	mod_name ## _logger_ctx.file = f;

#define LOG_EM(...)    logger_log(get_logger_ctx(), LOG_EMERG,  LOG_TAG, __VA_ARGS__)
#define LOG_ALERT(...) logger_log(get_logger_ctx(), LOG_ALERT,  LOG_TAG, __VA_ARGS__)
#define LOG_CRIT(...)  logger_log(get_logger_ctx(), LOG_CRIT,   LOG_TAG, __VA_ARGS__)
#define LOG_ERR(...)   logger_log(get_logger_ctx(), LOG_ERR,    LOG_TAG, __VA_ARGS__)
#define LOG_INF(...)   logger_log(get_logger_ctx(), LOG_INFO,   LOG_TAG, __VA_ARGS__)
#define LOG_WRN(...)   logger_log(get_logger_ctx(), LOG_WARNING,LOG_TAG, __VA_ARGS__)
#define LOG_NOT(...)   logger_log(get_logger_ctx(), LOG_NOTICE, LOG_TAG, __VA_ARGS__)
#define LOG_DBG(...)   logger_log(get_logger_ctx(), LOG_DEBUG,  LOG_TAG, __VA_ARGS__)

#endif /* _LOGGER_H_ */
