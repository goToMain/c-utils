#include <assert.h>
#include <string.h>

#include <utils/utils.h>
#include <utils/strutils.h>
#include <utils/hashmap.h>

#define HASH_MAP_BASE_SIZE            32
#define HASH_MAP_DENSITY_FACTOR       0.75
#define MAP_DENSITY(map) \
		((double)(map)->occupancy / (double)(map)->capacity)
#define GET_POOL_POS(map, hash) \
		(hash & (map->capacity - 1))

static size_t hash_map_count(hash_map_t *map)
{
	hash_map_item_t *item;
	size_t count = 0;

	for (size_t i = 0; i < map->capacity; i++) {
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
	assert(count == map->occupancy);
}

static void hash_map_rehash(hash_map_t *map)
{
	size_t pivot, target, old_capacity;
	hash_map_item_t *temp, *prev, *item;

	/* grow pool */
	old_capacity = map->capacity;
	map->capacity <<= 1; /* double capacity each time */
	map->pool = safe_realloc_zero(map->pool,
			old_capacity  * sizeof(hash_map_item_t *),
			map->capacity * sizeof(hash_map_item_t *));
	/* re-hash */
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
	map->occupancy = 0;
}

void hash_map_free(hash_map_t *map, hash_map_callback_t callback)
{
	size_t pos = 0;
	hash_map_item_t *item, *next;

	while (pos < map->capacity) {
		item = map->pool[pos];
		while (item != NULL) {
			callback(map->pool[pos]->key, map->pool[pos]->val);
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

void hash_map_insert(hash_map_t *map, const char *key, void *val)
{
	uint32_t hash, pos;
	hash_map_item_t *prev, *item;

	if (MAP_DENSITY(map) > HASH_MAP_DENSITY_FACTOR)
		hash_map_rehash(map);

	hash = hash32(key, -1);
	pos = GET_POOL_POS(map, hash);
	item = prev = map->pool[pos];
	while (item != NULL) {
		if (item->hash == hash && strcmp(item->key, key) == 0) {
			/* key already exists update it */
			item->val = val;
			return;
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
	map->occupancy += 1;
}

void *hash_map_get(hash_map_t *map, const char *key)
{
	uint32_t hash, pos;
	hash_map_item_t *item;

	hash = hash32(key, -1);
	pos = GET_POOL_POS(map, hash);
	item = map->pool[pos];
	while (item != NULL) {
		if (item->hash == hash && strcmp(item->key, key) == 0)
			return item->val;
		item = item->next;
	}
	return NULL;
}

void *hash_map_delete(hash_map_t *map, const char *key)
{
	void *val = NULL;
	uint32_t hash, pos;
	hash_map_item_t *prev = NULL, *item;

	hash = hash32(key, -1);
	pos = GET_POOL_POS(map, hash);
	item = map->pool[pos];
	while (item != NULL) {
		if (item->hash == hash && strcmp(item->key, key) == 0)
			break;
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
		map->occupancy -= 1;
	}
	return val;
}

void hash_map_foreach(hash_map_t *map, hash_map_callback_t callback)
{
	size_t pos = 0;
	hash_map_item_t *item;

	while (pos < map->capacity) {
		item = map->pool[pos];
		while (item != NULL) {
			callback(item->key, item->val);
			item = item->next;
		}
		pos += 1;
	}
}
