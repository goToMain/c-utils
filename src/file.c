/*
 * Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

#include <utils/file.h>

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
				free(data);
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
