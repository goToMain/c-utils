/*
 * Copyright (c) 2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _UTILS_SOCKUTILS_H_
#define _UTILS_SOCKUTILS_H_

int unix_socket_listen(const char *path, int max_clients);
int unix_socket_connect(const char *path);

#endif /* _UTILS_SOCKUTILS_H_ */