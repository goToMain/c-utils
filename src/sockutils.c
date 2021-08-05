/*
 * Copyright (c) 2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <utils/sockutils.h>

int unix_socket_listen(const char *path, int max_clients)
{
	int rc, fd;
	socklen_t len;
	struct sockaddr_un sock_serv;

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd < 0)
		return -1;

	sock_serv.sun_family = AF_UNIX;
	strcpy(sock_serv.sun_path, path);
	len = sizeof(sock_serv.sun_family) + strlen(path) + 1;
	unlink(path);

	rc = bind(fd, (const struct sockaddr *)&sock_serv, len);
	if (rc < 0)
		return -1;

	rc = listen(fd, max_clients);
	if (rc < 0)
		return -1;

	return fd;
}

int unix_socket_connect(const char *path)
{
	int rc, fd;
	socklen_t len;
	struct sockaddr_un serv_addr;

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd < 0)
		return -1;

	serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, path);
	len = sizeof(serv_addr.sun_family) + strlen(serv_addr.sun_path) + 1;

	rc = connect(fd, (struct sockaddr *)&serv_addr, len);
	if (rc != 0)
		return -1;

	return fd;
}