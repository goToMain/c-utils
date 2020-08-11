#include <string.h>
#include <utils/hashmap.h>
#include <utils/utils.h>

#include "test.h"

const char *test_hash_key[] = {
	"ambivert", "calcspar", "deaness", "equity", "grades", "monkeydom",
	"outclimbed", "outdared", "pistoleers", "redbugs", "snake-line",
	"subrules", "subtrends", "torture", "undefined", "endeavor", "stuff",
	"dissuade", "ecstatic", "properly", "saw", "entirely", "laughter",
	"jointure", "horrible", "margaret", "suitable", "followed", "stairs",
	"speedily.", "Indeed", "vanity", "excuse", "lovers", "offer", "pointed",
	"blush", "Sang", "lose", "hour", "then", "left", "find", "scale",
	"arrival", "entered", "drawing", "request", "daughters", "promotion",
	"knowledge", "contented.", "winter", "law", "behind", "number",
	"garret", "excuse.", "Minuter", "natural", "conduct", "gravity",
	"immediate", "unwilling", "attempted", "admitting", "disposing",
	"handsome", "opinions", "ladyship",
	NULL
};

struct test_hashmap {
	int off;
	char *key;
};

struct test_hashmap *new_test_hashmap_value(int i)
{
	struct test_hashmap *p;

	p = safe_malloc(sizeof(struct test_hashmap));
	p->off = i;
	p->key = safe_strdup(test_hash_key[i]);
	return p;
}

void free_test_hashmap_value(const char *key, void *val)
{
	struct test_hashmap *p = val;

	ARG_UNUSED(key);
	safe_free(p->key);
	safe_free(p);
}

int test_insert(hash_map_t *map)
{
	size_t i = 0;
	struct test_hashmap *p;

	while (test_hash_key[i] != NULL) {
		if (hash_map_get(map, test_hash_key[i]) == NULL) {
			p = new_test_hashmap_value(i);
			hash_map_insert(map, test_hash_key[i], p);
		} else {
			mod_printf("repeated word '%s'", test_hash_key[i]);
		}
		i += 1;
	}
	if (map->occupancy != i) {
		mod_printf("Error occupancy: %zu", map->occupancy);
		return -1;
	}
	return 0;
}

int test_delete(hash_map_t *map)
{
	int not_found = 0;
	int i = 0;
	struct test_hashmap *p;

	while (test_hash_key[i] != NULL) {
		p = hash_map_delete(map, test_hash_key[i]);
		if (p == NULL) {
			not_found += 1;
			mod_printf("word '%s' not found", test_hash_key[i]);
		} else {
			if (p->off != i || strcmp(p->key, test_hash_key[i])) {
				not_found += 1;
				mod_printf("word '%s' invalid offset %d",
					   test_hash_key[i], i);
			}
			free_test_hashmap_value(NULL, p);
		}
		i += 1;
	}
	return not_found * -1;
}

void do_test_hashmap(test_result_t *result)
{
	hash_map_t map;
	int total=0, pass=0;

	hash_map_init(&map);

	if (test_insert(&map) == 0) pass++; total++;
	if (test_delete(&map) == 0) pass++; total++;

	hash_map_free(&map, free_test_hashmap_value);

	result->pass = pass;
	result->total = total;
}
