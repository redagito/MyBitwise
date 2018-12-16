#pragma once

#include "hash.h"

/**
* Hashmap
*/

typedef struct map_t
{
	hash_t* keys;
	uint64_t* values;
	size_t length;
	size_t capacity;
};