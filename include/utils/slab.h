/*
 * Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _UTILS_SLAB_H_
#define _UTILS_SLAB_H_

#include <stddef.h>
#include <stdint.h>

#include <utils/utils.h>

typedef struct {
	uint8_t *blob;
	size_t blob_size;
	size_t block_size;
	size_t num_blocks;
	uint32_t *alloc_map;
} slab_t;

#define SLAB_DEF(name, type, count)                            \
	uint8_t name ## _slab_blob[sizeof(type) * count];      \
	uint32_t name ## _slab_alloc_map[(count + 31) / 32];   \
	slab_t name = {                                        \
		.blob = name ## _slab_blob,                    \
		.blob_size = sizeof(type),                     \
		.num_blocks = count,                           \
		.alloc_map = name ## _slab_alloc_map,          \
	};

int slab_init(slab_t *slab, size_t block_size, size_t num_blocks);
void slab_del(slab_t *slab);

int slab_alloc(slab_t *s, void **block);
int slab_free(slab_t *slab, void *block);

#endif /* _UTILS_SLAB_H_ */
