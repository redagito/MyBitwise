#include "syl/common/map.h"

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "syl/common/memory.h"
#include "syl/common/hash.h"

#define IS_POW2(x) (((x) != 0) && ((x) & ((x)-1)) == 0)

// Minimum capacity must be power of 2
#define MAP_MIN_CAPACITY 64

map_t map_create()
{
	map_t map;
	map.keys = NULL;
	map.values = NULL;
	map.size = 0;
	map.capacity = 0;

	return map;
}

/**
* Internal API
* Grow map to new capacity
*/
static void map_grow(map_t* map, size_t new_capacity)
{
	// Set at least to minimum capacity
	new_capacity = max(new_capacity, MAP_MIN_CAPACITY);
	if (new_capacity <= map->capacity)
	{
		return;
	}

	// Create map with new capacity
	map_t new_map = map_create();
	new_map.keys = xcalloc(new_capacity, sizeof(uint64_t));
	new_map.values = xmalloc(new_capacity * sizeof(uint64_t));
	new_map.capacity = new_capacity;

	// Copy content of old map
	for (size_t i = 0; i < map->capacity; ++i)
	{
		if (map->keys[i] != 0)
		{
			map_put_uint64_from_uint64(&new_map, map->keys[i], map->values[i]);
		}
	}

	free(map->keys);
	free(map->values);
	*map = new_map;
}

void* map_get(map_t* map, const void* key)
{
	return (void*)(uintptr_t)map_get_uint64_from_uint64(map, (uint64_t)(uintptr_t)key);
}

void map_put(map_t* map, const void* key, void* value)
{
	map_put_uint64_from_uint64(map, (uint64_t)(uintptr_t)key, (uint64_t)(uintptr_t)value);
}

void* map_get_from_uint64(map_t* map, uint64_t key)
{
	return (void*)(uintptr_t)map_get_uint64_from_uint64(map, key);
}

void map_put_from_uint64(map_t* map, uint64_t key, void* value)
{
	map_put_uint64_from_uint64(map, key, (uint64_t)(uintptr_t)value);
}

uint64_t map_get_uint64(map_t *map, const void* key)
{
	return map_get_uint64_from_uint64(map, (uint64_t)(uintptr_t)key);
}

void map_put_uint64(map_t* map, const void* key, uint64_t value)
{
	map_put_uint64_from_uint64(map, (uint64_t)(uintptr_t)key, value);
}

uint64_t map_get_uint64_from_uint64(map_t* map, uint64_t key)
{
	if (map->size == 0)
	{
		return 0;
	}
	// Validate map
	assert(IS_POW2(map->capacity));
	// Map must NOT be full or we will loop indefinitely
	// if the key does not exist
	assert(map->size < map->capacity);

	// Search for key
	size_t index = (size_t)hash_uint64(key);
	while (true)
	{
		index &= map->capacity - 1;
		if (map->keys[index] == key)
		{
			// Key found at index
			return map->values[index];
		}
		else if (map->keys[index] == 0)
		{
			// Key empty at index, requested key not in map
			return 0;
		}
		// Try next position
		index++;
	}
	return 0;
}

void map_put_uint64_from_uint64(map_t *map, uint64_t key, uint64_t value)
{
	assert(key != 0);
	if (value == 0)
	{
		return;
	}

	// Grow if half full
	if (2 * map->size >= map->capacity)
	{
		map_grow(map, 2 * map->capacity);
	}

	// Validate map
	assert(2 * map->size < map->capacity);
	assert(IS_POW2(map->capacity));

	// Insert value
	size_t index = (size_t)hash_uint64(key);
	while (true)
	{
		index &= map->capacity - 1;
		if (map->keys[index] == 0)
		{
			// Found empty position
			map->size++;
			map->keys[index] = key;
			map->values[index] = value;
			return;
		}
		else if (map->keys[index] == key)
		{
			// Key already present in map, update value
			map->values[index] = value;
			return;
		}
		// Try next position
		++index;
	}
}