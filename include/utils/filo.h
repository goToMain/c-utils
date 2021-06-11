#ifndef _UTILS_FILO_H_
#define _UTILS_FILO_H_

/*
 * Copyright (c) 2020-2021 Abhishek Arora <abhiarora@gotomain.io>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    void *buffer;
    size_t top;
    size_t elem_size;
    size_t max_size;
} filo_t;

#define __FILO_DEF(name, type, sz) \
    type name ## _buffer[sz]; \
    filo_t name = { .buffer = name ## _buffer, \
                     .top = 0, \
                     .elem_size = sizeof(type),\
                     .max_size = sz \
                  }; \

#define FILO_DEF(name, type, sz) \
    __FILO_DEF(name, type, sz) \
    int __## name ## _push(const type *elem) \
    { \
        return filo_push(&name, (void *)elem); \
    } \
    int __## name ## _pop(type *elem) \
    { \
        return filo_pop(&name, (void *)elem, true); \
    } \
    int __## name ## _peek(type *elem) \
    { \
        return filo_pop(&name, (void *)elem, false); \
    } \
    unsigned __## name ## _get_count(void) \
    { \
        return filo_get_count(&name); \
    } \
    void __## name ## _reset(void) \
    { \
        filo_reset(&name); \
    }

#define FILO_EXTERN(name, type) \
    extern filo_t name; \
    int __## name ## _push(const type *elem); \
    int __## name ## _pop(type *elem); \
    int __## name ## _peek(type *elem); \
    unsigned __## name ## _get_count(void); \
    void __## name ## _reset(void);


/**
 * Description:
 *   Initializes the filo_t pointed to by `pfilo`.
 *
 * Returns: None
 */
void filo_init(filo_t *pfilo, void *buffer, size_t elem_size, size_t max_size);

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
 *   Pushes element pointed to by `elem` at the top of filo `name`.
 *   This is read-write method, occupancy count increases by one.
 *
 * Returns (int):
 *   0 - Success
 *  -1 - Out of space
 */
#define FILO_PUSH(name, elem)             __## name ## _push(elem)

/**
 * Description:
 *   Removes (if remove is true) the element at top from filo `pfilo` and makes it
 *   available at `elem`. This is read-write method (if remove is true, 
 *   otherwise it is just read method), occupancy count reduces by one.
 *
 * Returns (int):
 *   0 - Success
 *  -1 - Empty
 */
int filo_pop(filo_t *pfilo, void *elem, bool remove);

/**
 * Description:
 *   Peeks the element at top from filo `name` and makes it
 *   available at `elem`.
 *
 * Returns (int):
 *   0 - Success
 *  -1 - Empty
 */
#define FILO_PEEK(name, elem)             __## name ## _peek(elem)

/**
 * Description:
 *   Removes the element at top from filo `name` and makes it
 *   available at `elem`. This is read-write method, 
 *   occupancy count reduces by one.
 *
 * Returns (int):
 *   0 - Success
 *  -1 - Empty
 */
#define FILO_POP(name, elem)              __## name ## _pop(elem)


/**
 * Description:
 *   Returns the occupancy count of filo 'pfilo'.
 *
 * Returns (int):
 *   0 - Success
 *   0..N - number of slots occupied.
 */
size_t filo_get_count(filo_t *pfilo);

/**
 * Description:
 *   Returns the occupancy count of filo 'name'.
 *
 * Returns (int):
 *   0 - Success
 *   0..N - number of slots occupied.
 */
#define FILO_GET_COUNT(name)                     __## name ## _get_count()

/**
 * Description:
 *   Returns the number of free slots of filo 'pfilo'.
 *
 * Returns (int):
 *   0 - Success
 *   0..N - number of slots free.
 */
size_t filo_get_free_space(filo_t *pfilo);

/**
 * Description:
 *   Dynamically allocate the filo having elem_size of 'elem_size' and maximum number of
 *   elements 'max_size'.
 *
 * Returns (int):
 *   0 - Success
 *   0..N - number of slots occupied.
 */
filo_t *filo_alloc(size_t elem_size, size_t max_size);

/**
 * Description:
 *   Frees dynamically allocated filo 'pfilo'.
 */
void filo_free(filo_t *pfilo);



#endif /* _UTILS_FILO_H_ */
