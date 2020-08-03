/*
 * Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _UTILS_UTILS_H_
#define _UTILS_UTILS_H_

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

#define ARG_UNUSED(x)                  (void)(x)

#define ARRAY_SIZEOF(x) \
	(sizeof(x) / sizeof(x[0]))

#define ARRAY_BASE(ptr, type, offset) \
	((char *)(ptr)) - ((sizeof(type)) * offset)

#define OFFSET_OF(type, field) (size_t)(&((type *)(0))->field)

#define CONTAINER_OF(ptr, type, field) \
        ((type *)(((char *)(ptr)) - OFFSET_OF(type, field)))

/**
 * @brief Check p to be not NULL before calling safe_free()
 *
 * @param p pointer to be free-ed
 */
void safe_free(void *p);

/**
 * @brief safe_* variants of the standard alloc methods do a check on the
 * returned pointer and will call exit() if any of the returned NULL. The
 * safe_free() method will check if pointer is NULL before calling safe_free().
 */
void  safe_free(void *p);
void *safe_malloc(size_t size);
void *safe_calloc(size_t count, size_t size);
void *safe_realloc(void *data, size_t size);
void *safe_strdup(const char *s);

#endif /* _UTILS_UTILS_H_ */
