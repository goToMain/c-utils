#include <assert.h>
#include <string.h>

#include <utils/utils.h>
#include <utils/hashmap.h>

#define HASH_MAP_BASE_SIZE            32
#define HASH_MAP_DENSITY_FACTOR       0.75
#define HASH_MAP_GROWTH_FACTOR        2
#define MAP_DENSITY(map) \
		((double)(map)->occupancy / (double)(map)->capacity)

static uint32_t hash32(const char *str)
{
	int c;
	uint32_t hash = 5381;

	while ((c = *str++)) {
		hash = ((hash << 5) + hash) ^ c; /* hash * 33 ^ c */
	}
	return hash;
}

static void hash_map_rehash(hash_map_t *map)
{
	size_t pivot, target, old_capacity;
	hash_map_item_t *temp, *prev, *item, *next;

	/* grow pool */
	old_capacity = map->capacity;
	map->capacity *= HASH_MAP_GROWTH_FACTOR;
	map->pool = safe_realloc(map->pool,
			map->capacity * sizeof(hash_map_item_t *));
	/* re-hash */
	for (pivot = 0; pivot < old_capacity; pivot++) {
		prev = NULL;
		item = map->pool[pivot];
		while (item != NULL) {
			target = item->hash % map->capacity;
			if (pivot != target) {
				/* remove item from pivot */
				if (prev != NULL)
					prev->next = item->next;
				else
					map->pool[pivot] = item->next;
				next = item->next;
				/* prepend item, to target.head */
				temp = map->pool[target];
				map->pool[target] = item;
				map->pool[target]->next = temp;
				item = next;
			} else {
				prev = item;
				item = item->next;
			}
		}
	}
}

void hash_map_init(hash_map_t *map)
{
	map->pool = safe_calloc(HASH_MAP_BASE_SIZE, sizeof(hash_map_item_t *));
	map->capacity = HASH_MAP_BASE_SIZE;
	map->occupancy = 0;
}

void hash_map_free(hash_map_t *map, hash_map_callback_t callback)
{
	size_t pos = 0;
	hash_map_item_t *item;

	while (pos < map->capacity) {
		item = map->pool[pos];
		while (item != NULL) {
			callback(map->pool[pos]->key, map->pool[pos]->val);
			safe_free(item->key);
			safe_free(item);
			item = item->next;
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
	uint32_t hash;
	hash_map_item_t *prev, *item;

	if (MAP_DENSITY(map) > HASH_MAP_DENSITY_FACTOR)
		hash_map_rehash(map);

	hash = hash32(key);
	item = prev = map->pool[hash % map->capacity];
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
		map->pool[hash % map->capacity] = item;
	map->occupancy += 1;
}

void *hash_map_get(hash_map_t *map, const char *key)
{
	uint32_t hash;
	hash_map_item_t *item;

	hash = hash32(key);
	item = map->pool[hash % map->capacity];
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
	uint32_t hash;
	hash_map_item_t *prev = NULL, *item;

	hash = hash32(key);
	item = map->pool[hash % map->capacity];
	while (item != NULL) {
		if (item->hash == hash && strcmp(item->key, key) == 0)
			break;
		prev = item;
		item = item->next;
	}
	if (item != NULL) { /* key found */
		val = item->val;
		safe_free(item->key);
		safe_free(item);
		if (prev != NULL)
			prev->next = item->next;
		else
			map->pool[hash % map->capacity] = item->next;
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
			callback(map->pool[pos]->key, map->pool[pos]->val);
			item = item->next;
		}
		pos += 1;
	}
}
