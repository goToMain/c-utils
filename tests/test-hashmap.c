#include <string.h>
#include <utils/hashmap.h>
#include <utils/memory.h>
#include <utils/strutils.h>

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

int test_dict_insert(hash_map_t *map, const char * const *words, size_t count)
{
	size_t ndx = 0;
	struct test_hashmap *p;

	while (ndx < count) {
		p = new_test_hashmap_value(ndx, words[ndx]);
		hash_map_insert(map, words[ndx], p);
		ndx++;
	}
	mod_printf("Inserted %zu items", ndx);
	return 0;
}

int test_dict_iterator(hash_map_t *map, const char * const *words, size_t count)
{
	size_t ndx = 0, mismatch = 0;
	struct test_hashmap *val;
	char *key;

	HASH_MAP_FOREACH(map, &key, &val) {
		if (strcmp(val->key, words[val->off])) {
			mismatch += 1;
			mod_printf("Err: word mismatch '%s/%s' off: %zu",
				   words[ndx], val->key, val->off);
		}
		ndx++;
	}
	if (ndx != count || mismatch)
		return -1;
	mod_printf("Iterated over %zu items", ndx);
	return 0;
}

int test_dict_delete(hash_map_t *map, const char * const *words, size_t count)
{
	size_t ndx = 0, not_found = 0;
	struct test_hashmap *p;

	while (ndx < count) {
		p = hash_map_delete(map, words[ndx], 0);
		if (p == NULL) {
			not_found += 1;
			mod_printf("word '%s' not found", words[ndx]);
		} else {
			if (p->off != ndx || strcmp(p->key, words[ndx])) {
				not_found += 1;
				mod_printf("Err: word '%s/%s' off: %zu",
					   words[ndx], p->key, ndx);
			}
			free_test_hashmap_value(NULL, p);
		}
		ndx++;
	}
	mod_printf("Deleted %zu items", ndx);
	return not_found * -1;
}

size_t test_build_word_list(char *buf, const char *const **word_list)
{
	size_t count, ndx = 0;
	char *word, *state, **words;

	count = str_sep_count(buf, "\n\r \t");
	words = safe_malloc(sizeof(char *) * count);

	word = strtok_r(buf, SEP, &state);
	while (ndx < count && word != NULL) {
		words[ndx] = word;
		word = strtok_r(NULL, SEP, &state);
		ndx += 1;
	}
	*word_list = (const char *const *)words;
	return count;
}

TEST_DEF(hashmap)
{
	char *buf;
	const char * const *words;
	size_t size, count;
	hash_map_t map;
	TEST_MOD_INIT();

	TEST_MOD_READ_FILE("words_alpha.txt", &buf, &size);
	count = test_build_word_list(buf, &words);

	hash_map_init(&map);

	TEST_MOD_EXEC( test_dict_insert(&map, words, count) );
	TEST_MOD_EXEC( test_dict_iterator(&map, words, count) );
	TEST_MOD_EXEC( test_dict_delete(&map, words, count) );
	TEST_MOD_EXEC( map.count );

	TEST_MOD_EXEC( test_dict_insert(&map, words, count) );
	hash_map_free(&map, free_test_hashmap_value);

	safe_free(buf);
	safe_free((void *)words);
	TEST_MOD_REPORT();
}
