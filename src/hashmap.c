#include <assert.h>
#include <string.h>
#include <stdio.h>

#include <utils/utils.h>
#include <utils/memory.h>
#include <utils/strutils.h>
#include <utils/hashmap.h>

#define hash_fn hash32_djb2

#define HASH_MAP_BASE_SIZE            32
#define HASH_MAP_DENSITY_FACTOR       0.8
#define MAP_DENSITY(map) \
		((double)(map)->count / (double)(map)->capacity)
#define GET_POOL_POS(map, hash) \
		(hash & (map->capacity - 1))

#if 0
#define hash_check_key(m, i, k) \
	if (strcmp(i->key, k) != 0) { \
		printf("Error: Hash collusion in %s for %s/%s\n", m, k, i->key); \
	}
#else
#define hash_check_key(m, i, k)
#endif

static size_t hash_map_count(hash_map_t *map)
{
	size_t i, count = 0;
	hash_map_item_t *item;

	for (i = 0; i < map->capacity; i++) {
		item = map->pool[i];
		while (item) {
			count += 1;
			item = item->next;
		}
	}
	return count;
}

static void hash_map_lint(hash_map_t *map)
{
	size_t count;

	count = hash_map_count(map);
	ARG_UNUSED(count); /* for release builds */
	assert(count == map->count);
}

static void hash_map_rehash(hash_map_t *map)
{
	size_t pivot, target, old_capacity;
	hash_map_item_t *temp, *prev, *item;

	old_capacity = map->capacity;
	map->capacity <<= 1; /* double capacity each time */
	map->pool = safe_realloc_zero(map->pool,
			old_capacity  * sizeof(hash_map_item_t *),
			map->capacity * sizeof(hash_map_item_t *));

	/* spread out the hashes in the range [0 .. old_capacity] */
	for (pivot = 0; pivot < old_capacity; pivot++) {
		prev = NULL;
		item = map->pool[pivot];
		while (item != NULL) {
			target = GET_POOL_POS(map, item->hash);
			assert(target < map->capacity);
			if (pivot != target) {
				/* remove item from pivot */
				if (prev != NULL)
					prev->next = item->next;
				else
					map->pool[pivot] = item->next;
				temp = item->next;
				/* prepend item, to target.head */
				item->next = map->pool[target];
				map->pool[target] = item;
				item = temp;
			} else {
				prev = item;
				item = item->next;
			}
		}
	}
	hash_map_lint(map);
}

void hash_map_init(hash_map_t *map)
{
	size_t size;

	size = round_up_pow2(HASH_MAP_BASE_SIZE);
	map->pool = safe_calloc(size, sizeof(hash_map_item_t *));
	map->capacity = size;
	map->count = 0;
}

void hash_map_free(hash_map_t *map, hash_map_callback_t callback)
{
	size_t pos = 0;
	hash_map_item_t *item, *next;

	while (pos < map->capacity) {
		item = map->pool[pos];
		while (item != NULL) {
			callback(item->key, item->val);
			next = item->next;
			safe_free(item->key);
			safe_free(item);
			item = next;
		}
		pos += 1;
	}
	safe_free(map->pool);
	map->pool = NULL;
	map->capacity = 0;
	map->capacity = 0;
}

hash_t hash_map_insert(hash_map_t *map, const char *key, void *val)
{
	hash_t hash;
	size_t pos;
	hash_map_item_t *prev, *item;

	if (MAP_DENSITY(map) > HASH_MAP_DENSITY_FACTOR)
		hash_map_rehash(map);

	hash = hash_fn(key, -1);
	pos = GET_POOL_POS(map, hash);
	item = prev = map->pool[pos];
	while (item != NULL) {
		if (item->hash == hash) {
			/* key already exists update it */
			hash_check_key("insert", item, key);
			if (strcmp(item->key, key) == 0) {
				item->val = val;
				return item->hash;
			}
		}
		prev = item;
		item = item->next;
	}

	/* insert new key */
	item = safe_malloc(sizeof(hash_map_item_t));
	item->hash = hash;
	item->key = safe_strdup(key);
	item->val = val;
	item->next = NULL;
	if (prev != NULL)
		prev->next = item;
	else
		map->pool[pos] = item;
	map->count += 1;
	return item->hash;
}

void *hash_map_get(hash_map_t *map, const char *key, hash_t key_hash)
{
	hash_t hash;
	size_t pos;
	hash_map_item_t *item;

	hash = key ? hash_fn(key, -1) : key_hash;
	pos = GET_POOL_POS(map, hash);
	item = map->pool[pos];
	while (item != NULL) {
		if (item->hash == hash) {
			hash_check_key("get", item, key);
			if (strcmp(item->key, key) == 0) {
				return item->val;
			}
		}
		item = item->next;
	}
	return NULL;
}

void *hash_map_delete(hash_map_t *map, const char *key, hash_t key_hash)
{
	void *val = NULL;
	hash_t hash;
	size_t pos;
	hash_map_item_t *prev = NULL, *item;

	hash = key ? hash_fn(key, -1) : key_hash;
	pos = GET_POOL_POS(map, hash);
	item = map->pool[pos];
	while (item != NULL) {
		if (item->hash == hash) {
			hash_check_key("delete", item, key);
			if (strcmp(item->key, key) == 0) {
				break;
			}
		}
		prev = item;
		item = item->next;
	}
	if (item != NULL) { /* key found */
		val = item->val;
		if (prev != NULL)
			prev->next = item->next;
		else
			map->pool[pos] = item->next;
		safe_free(item->key);
		safe_free(item);
		map->count -= 1;
	}
	return val;
}

void hash_map_it_init(hash_map_iterator_t *it, hash_map_t *map)
{
	it->map = map;
	it->item = NULL;
	it->pos = 0;
}

int hash_map_it_next(hash_map_iterator_t *it, char **key, void **val)
{
	hash_map_item_t *cur;

	if (it->item != NULL) {
		cur = it->item;
		it->item = cur->next;
	} else {
		while (it->pos < it->map->capacity &&
		       it->map->pool[it->pos] == NULL)
			it->pos++;
		if (it->pos >= it->map->capacity ||
		    it->map->pool[it->pos] == NULL)
			return -1;
		cur = it->map->pool[it->pos];
		it->item = cur->next;
		it->pos++;
	}
	*key = cur->key;
	*val = cur->val;
	return 0;
}
