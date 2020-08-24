#ifndef _UTILS_FILO_H_
#define _UTILS_FILO_H_

/*
 * Copyright (c) 2020 Abhishek Arora <abhiarora@gotomain.io>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    void *buffer;
    unsigned top;
    unsigned elem_size;
    unsigned max_size;
} filo_t;

#define FILO_DEF(type, name, sz) \
    type name_##buffer[sz]; \
    filo_t name = { .buffer = name ## _buffer, \
                     .top = 0, \
                     .elem_size = sizeof(type),\
                     .max_size = sz \
                    }; \

#define FILO_GLOB_DEF(type, name, sz) \
    FILO_DEF(type, name, sz) \
    int name ## _push(const type *elem) \
    { \
        return filo_push(&name, (void *)elem); \
    } \
    int name ## _pop(const type *elem) \
    { \
        return filo_pop(&name, (void *)elem, true); \
    } \
    int name ## _peek(const type *elem) \
    { \
        return filo_pop(&name, (void *)elem, false); \
    } \
    unsigned name ## _get_count(const type *elem) \
    { \
        return filo_get_count(&name); \
    } \
    int name ## _reset(const type *elem) \
    { \
        return filo_reset(&name); \
    } \

/**
 * Description:
 *   Initializes the filo_t pointed to by `pfilo`.
 *
 * Returns: None
 */
void filo_init(filo_t *pfilo, void *buffer, unsigned elem_size, unsigned max_size);

/**
 * Description:
 *   Resets the filo offsets to zero. Does not clean the newly freed
 *   slots.
 */
void filo_reset(filo_t *pfilo);

/**
 * Description:
 *   Pushes element pointed to by `elem` at the top of filo `pfilo`.
 *   This is read-write method, occupancy count increases by one.
 *
 * Returns (int):
 *   0 - Success
 *  -1 - Out of space
 */
int filo_push(filo_t *pfilo, void *elem);

/**
 * Description:
 *   Removes the element at top from filo `pfilo` and makes it
 *   available at `elem`. This is read-write method, occupancy count reduces
 *   by one.
 *
 * Returns (int):
 *   0 - Success
 *  -1 - Empty
 */
int filo_pop(filo_t *pfilo, void *elem, bool remove);


/**
 * Description:
 *   Returns the occupancy count of filo 'pfilo'
 *   by one.
 *
 * Returns (int):
 *   0 - Success
 *   0..N - number of slots occupied.
 */
unsigned filo_get_count(filo_t *pfilo);

/**
 * Description:
 *   Dynamically allocate the filo having elem_size of 'elem_size' and maximum number of
 *   elements 'max_size'.
 *
 * Returns (int):
 *   0 - Success
 *   0..N - number of slots occupied.
 */
filo_t *filo_alloc(unsigned elem_size, unsigned max_size);

/**
 * Description:
 *   Frees dynamically allocated filo 'pfilo'.
 */
void filo_free(filo_t *pfilo);



#endif /* _UTILS_FILO_H_ */