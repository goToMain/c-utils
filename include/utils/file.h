/*
 * Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _UTIL_FILE_H_
#define _UTIL_FILE_H_

#include <stdio.h>

int file_read_all(FILE *in, char **dataptr, size_t *sizeptr);

#endif /* _UTIL_FILE_H_ */
