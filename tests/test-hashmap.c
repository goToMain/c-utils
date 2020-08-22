#include <string.h>
#include <utils/hashmap.h>
#include <utils/utils.h>

#include "test.h"

struct test_hashmap {
	size_t off;
	char *key;
};

struct test_hashmap *new_test_hashmap_value(size_t offset, const char *key)
{
	struct test_hashmap *p;

	p = safe_malloc(sizeof(struct test_hashmap));
	p->off = offset;
	p->key = safe_strdup(key);
	return p;
}

void free_test_hashmap_value(const char *key, void *val)
{
	struct test_hashmap *p = val;

	ARG_UNUSED(key);
	safe_free(p->key);
	safe_free(p);
}

#define SEP "\n\r \t"

int test_dict_insert(hash_map_t *map, char *buf)
{
	size_t count = 0;
	struct test_hashmap *p;
	char *word, *state;

	word = strtok_r(buf, SEP, &state);
	while (word != NULL) {
		p = new_test_hashmap_value(count, word);
		hash_map_insert(map, word, p);
		count++;
		word = strtok_r(NULL, SEP, &state);
	}
	mod_printf("Inserted %zu items", count);
	return 0;
}

int test_dict_delete(hash_map_t *map, char *buf)
{
	size_t i = 0, not_found = 0;
	struct test_hashmap *p;
	char *word, *state;

	word = strtok_r(buf, SEP, &state);
	while (word != NULL) {
		p = hash_map_delete(map, word, 0);
		if (p == NULL) {
			not_found += 1;
			mod_printf("word '%s' not found", word);
		} else {
			if (p->off != i || strcmp(p->key, word)) {
				not_found += 1;
				mod_printf("Err: word '%s' off: %zu", word, i);
			}
			free_test_hashmap_value(NULL, p);
		}
		i++;
		word = strtok_r(NULL, SEP, &state);
	}
	return not_found * -1;
}

TEST_DEF(hashmap)
{
	char *buf_insert, *buf_delete;
	size_t size;
	hash_map_t map;
	TEST_MOD_INIT();

	TEST_MOD_READ_FILE("words_alpha.txt", &buf_insert, &size);
	buf_delete = safe_strdup(buf_insert);

	hash_map_init(&map);

	TEST_MOD_EXEC(test_dict_insert(&map, buf_insert));
	TEST_MOD_EXEC(test_dict_delete(&map, buf_delete));
	TEST_MOD_EXEC(map.count);

	hash_map_free(&map, free_test_hashmap_value);

	safe_free(buf_insert);
	safe_free(buf_delete);
	TEST_MOD_REPORT();
}
