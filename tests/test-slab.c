#include "test.h"

#include <utils/slab.h>

struct test_slab_blocks
{
	int test;
	long long long_test;
};

#define TEST_SLAB_COUNT 10

int test_slab_alloc_free()
{
	int i = 0;
	struct test_slab_blocks *p[TEST_SLAB_COUNT + 1];
	uint8_t slab_space[(sizeof(struct test_slab_blocks) + 8) * TEST_SLAB_COUNT];
	slab_t slab;

	slab_init(&slab, sizeof(struct test_slab_blocks),
		  slab_space, sizeof(slab_space));

	while (i < TEST_SLAB_COUNT) {
		if (slab_alloc(&slab, (void **)&p[i]))
			return -1;
		i++;
	}

	if (slab_alloc(&slab, (void **)&p[i]) == 0)
		return -1;

	while (--i < 0) {
		if (slab_free(&slab, p[i]))
			return -1;
	}

	if (slab_free(&slab, (void **)&p[0]) == 0)
		return -1;

	return 0;
}

TEST_DEF(slab)
{
	TEST_MOD_INIT();

	TEST_MOD_EXEC( test_slab_alloc_free() );

	TEST_MOD_REPORT();
}
