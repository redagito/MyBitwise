#pragma once

#include <stdint.h>

/**
* Arena allocator
*/

typedef struct arena_t
{
	char* ptr;
	char* end;
	char **blocks;
} arena_t;

void arena_grow(arena_t* arena, size_t min_size);

void* arena_alloc(arena_t* arena, size_t size);

void arena_free(arena_t* arena);