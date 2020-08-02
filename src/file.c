/*
 * Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <libgen.h>

#include <utils/file.h>
#include <utils/utils.h>

int file_read_all(FILE *in, char **dataptr, size_t *sizeptr)
{
	char *data = NULL, *temp;
	size_t n, size = 0, used = 0, chunk_size = (2 * 1024L);

	/* None of the parameters can be NULL. */
	if (in == NULL || dataptr == NULL || sizeptr == NULL) {
		return -1;
	}

	/* A read error already occurred? */
	if (ferror(in)) {
		return -1;
	}

	while (true) {
		if (used + chunk_size + 1 > size) {
			size = used + chunk_size + 1;
			/* size overflow check. Some ANSI C compilers may
			   optimize this away, though. */
			if (size <= used) {
				free(data);
				return -1;
			}

			temp = realloc(data, size);
			if (temp == NULL) {
				safe_free(data);
				return -1;
			}
			data = temp;
		}
		n = fread(data + used, 1, chunk_size, in);
		used += n;
		if (n < chunk_size) {
			break;
		}
		chunk_size *= 2; /* exponential read */
	}

	/* check if we broke from the while due to errors/non-eof conditions */
	if (ferror(in) != 0 || feof(in) == 0) {
		free(data);
		return -1;
	}

	temp = realloc(data, used + 1);
	if (temp == NULL) {
		free(data);
		return -1;
	}
	data = temp;
	data[used] = '\0';

	*dataptr = data;
	*sizeptr = used;

	return 0;
}

#define PATH_SEP '/'

char *path_join(const char *p1, const char *p2)
{
	char *path;
	size_t p1_len, p2_len;

	if (p2 == NULL)
		return NULL;
	if (p2[0] == PATH_SEP)
		return strdup(p2);
	p1_len = (p1 == NULL) ? 0 : strlen(p1);
	p2_len = strlen(p2);
	path = malloc(sizeof(char) * (p1_len + p2_len + 2));
	if (path) {
		if (p1) {
			strncpy(path, p1, p1_len);
		}
		if (p1_len && path[p1_len - 1] != PATH_SEP) {
			path[p1_len] = PATH_SEP;
			strncpy(path + p1_len + 1, p2, p2_len);
			path[p1_len + 1 + p2_len] = '\0';
		} else {
			strncpy(path + p1_len, p2, p2_len);
			path[p1_len + p2_len] = '\0';
		}
	}
	return path;
}

char *get_working_directory(void)
{
	char *p;

	p = malloc(sizeof(char) * PATH_MAX);
	if (p == NULL) {
		return NULL;
	}
	if (getcwd(p, PATH_MAX) == NULL) {
		free(p);
		return NULL;
	}
	return p;
}

int path_extract(const char *path, char **dir_name, char **base_name)
{
	char *dname = NULL, *bname = NULL, *rpath = NULL;

	if (path == NULL)
		return -1;

	rpath = realpath(path, NULL);
	if (rpath == NULL)
		return -1;

	if (dir_name) {
		dname = malloc(sizeof(char) * PATH_MAX);
		if (dname == NULL) {
			free(rpath);
			return -1;
		}
		if (dirname_r(rpath, dname) == NULL) {
			free(rpath);
			free(dname);
			return -1;
		}
		*dir_name = dname;
	}

	if (base_name) {
		bname = malloc(sizeof(char) * PATH_MAX);
		if (bname == NULL) {
			free(rpath);
			safe_free(dname);
			return -1;
		}
		if (basename_r(rpath, bname) == NULL) {
			free(rpath);
			free(bname);
			safe_free(dname);
			return -1;
		}
		*base_name = bname;
	}

	free(rpath);
	return 0;
}
