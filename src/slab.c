/*
 * Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <string.h>

#include <utils/slab.h>

int slab_init(slab_t *slab, size_t block_size, size_t num_blocks)
{
	slab->block_size = round_up_pow2(block_size);
	slab->blob = safe_calloc(num_blocks, slab->block_size);
	slab->alloc_map = safe_calloc((num_blocks + 31) / 32, sizeof(uint32_t));
	return 0;
}

void slab_del(slab_t *slab)
{
	safe_free(slab->blob);
	safe_free(slab->alloc_map);
	memset(slab, 0, sizeof(slab_t));
}

int slab_alloc(slab_t *slab, void **block)
{
	void *p;

	size_t i = 0, offset = 0;

	while (i < slab->num_blocks &&
	       slab->alloc_map[offset] & (1L << (i & 0x1f)))
	{
		if ((i & 0x1f) == 0x1f)
			offset++;
		i++;
	}
	if (i >= slab->num_blocks)
		return -1;
	slab->alloc_map[offset] |= 1L << (i & 0x1f);
	p = slab->blob + (slab->blob_size * i);
	memset(p, 0, slab->block_size);
	*block = p;
	return 0;
}

int slab_free(slab_t *slab, void *block)
{
	size_t i;

	for (i = 0; i < slab->num_blocks; i++) {
		if ((slab->blob + i) == block)
			break;
	}
	if (i >= slab->num_blocks)
		return -1;
	if (!(slab->alloc_map[i / 32] & (1L << (i & 0x1f))))
		return -2;
	slab->alloc_map[i / 32] &= ~(1L << (i & 0x1f));
	return 0;
}
