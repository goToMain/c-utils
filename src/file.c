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

#include <sys/types.h>
#include <sys/stat.h>

#include <utils/file.h>
#include <utils/utils.h>

int file_read_all(FILE *in, char **dataptr, size_t *sizeptr)
{
	char *data = NULL;
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
				safe_free(data);
				return -1;
			}
			data = safe_realloc(data, size);
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
		safe_free(data);
		return -1;
	}

	data = safe_realloc(data, used + 1);
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
		return safe_strdup(p2);
	p1_len = (p1 == NULL) ? 0 : strlen(p1);
	p2_len = strlen(p2);
	path = safe_malloc(sizeof(char) * (p1_len + p2_len + 2));
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
	return path;
}

char *get_working_directory(void)
{
	char *p;

	p = safe_malloc(sizeof(char) * PATH_MAX);
	if (getcwd(p, PATH_MAX) == NULL) {
		safe_free(p);
		return NULL;
	}
	return p;
}

int is_regular_file(const char *path)
{
	struct stat path_stat;
	stat(path, &path_stat);
	return S_ISREG(path_stat.st_mode);
}

int path_extract(const char *path, char **dir_name, char **base_name)
{
	int ret = -1;
	char *dname = NULL, *dname_copy = NULL;
	char *bname = NULL, *bname_copy = NULL, *rpath;

	if (!path || !is_regular_file(path))
		return -1;

	if (dir_name) *dir_name = NULL;
	if (base_name) *base_name = NULL;

	rpath = safe_malloc(sizeof(char) * PATH_MAX);
	if (realpath(path, rpath) == NULL) {
		perror("realpath");
		goto error;
	}
	if (dir_name) {
		dname_copy = safe_strdup(rpath);
		dname = dirname(rpath);
		if (dname == NULL) {
			perror("dirname");
			goto error;
		}
		*dir_name = safe_strdup(dname);
	}
	if (base_name) {
		bname_copy = safe_strdup(rpath);
		bname = basename(bname_copy);
		if (bname == NULL) {
			perror("basename");
			goto error;
		}
		*base_name = safe_strdup(bname);
	}
	ret = 0;
cleanup:
	safe_free(rpath);
	safe_free(dname_copy);
	safe_free(bname_copy);
	return ret;
error:
	if (dir_name)
		safe_free(*dir_name);
	if (base_name)
		safe_free(*base_name);
	goto cleanup;
}
