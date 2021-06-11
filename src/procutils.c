/*
 * Copyright (c) 2020-2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <utils/procutils.h>

int read_pid(const char *file, int *pid)
{
	int pid_val, ret = 0;
	FILE *fd;

	if (file == NULL)
		return -1;

	fd = fopen(file, "r");
	if (fd == NULL)
		return -1;
	if (fscanf(fd, "%d", &pid_val) != 1) {
		printf("Failed to read PID from file %s\n", file);
		ret = -1;
	}
	fclose(fd);

	if (pid)
		*pid = pid_val;
	return ret;
}

int write_pid(const char *file)
{
	FILE *fd;
	char pid_str[32];

	if (file == NULL)
		return 1;

	fd = fopen(file, "w");
	if (fd == NULL) {
		perror("Error opening pid_file to write");
		return -1;
	}
	snprintf(pid_str, 32, "%d\n", getpid());
	fputs(pid_str, fd);
	fclose(fd);
	return 0;
}

int o_redirect(int mode, const char *file)
{
	int fd = -1;
	int has_err = 0;

	if (file != NULL) {
		fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0644);
		if (fd == -1) {
			perror("opening log_file");
			return -1;
		}
	}

	if (mode & 0x01) {
		if (fd != -1) {
			if (dup2(fd, fileno(stdout)) == -1) {
				perror("cannot redirect stdout to log_file");
				has_err = -1;
			}
		} else {
			close(fileno(stdout));
		}
	}

	if (mode & 0x02) {
		if (fd != -1) {
			if (dup2(fd, fileno(stderr)) == -1) {
				perror("cannot redirect stderr to log_file");
				has_err = -1;
			}
		} else {
			close(fileno(stderr));
		}
	}

	return has_err;
}
