#pragma once

#include <stdint.h>

/**
 * Map for 64bit values / pointer hash table
 */

typedef struct map_t {
  uint64_t *keys;
  uint64_t *values;
  size_t size;     // Current size
  size_t capacity; // Maximum capacity
} map_t;

/**
 * Sets default values
 */
map_t map_create();

void *map_get(map_t *map, const void *key);
void map_put(map_t *map, const void *key, void *value);

void *map_get_from_uint64(map_t *map, uint64_t key);
void map_put_from_uint64(map_t *map, uint64_t key, void *value);

uint64_t map_get_uint64(map_t *map, const void *key);
void map_put_uint64(map_t *map, const void *key, uint64_t value);

uint64_t map_get_uint64_from_uint64(map_t *map, uint64_t key);
void map_put_uint64_from_uint64(map_t *map, uint64_t key, uint64_t value);