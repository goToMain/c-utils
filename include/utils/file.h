/*
 * Copyright (c) 2020-2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _UTIL_FILE_H_
#define _UTIL_FILE_H_

#include <stdio.h>

int file_read_all(FILE *in, char **dataptr, size_t *sizeptr);
char *get_working_directory(void);
char *path_join(const char *p1, const char *p2);
int is_regular_file(const char *path);
int path_extract(const char *path, char **dir_name, char **base_name);

#endif /* _UTIL_FILE_H_ */
