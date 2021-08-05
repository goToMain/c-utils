/*
 * Copyright (c) 2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _UTILS_FDUTILS_H_
#define _UTILS_FDUTILS_H_

#include <unistd.h>
#include <sys/fcntl.h>

int fcntl_setfl(int fd, int flag);

ssize_t read_loop(int fd, void *buf, size_t max_len);
ssize_t write_loop(int fd, const void *buf, size_t len);
int flush_fd(int fd);

#endif /* _UTILS_FDUTILS_H_ */
