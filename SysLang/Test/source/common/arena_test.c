#include "testy.h"

#include <syl/common/arena.h>

TESTY_TEST(arena)
{
	arena_t arena = arena_create();
	TESTY_ASSERT(arena.ptr == NULL);
	TESTY_ASSERT(arena.end == NULL);
	TESTY_ASSERT(arena.blocks == NULL);
	
	// Simple alloc
	int* ptr = NULL;
	ptr = arena_alloc(&arena, sizeof(int));
	TESTY_ASSERT(ptr != NULL);
	
	// Big alloc
	ptr = arena_alloc(&arena, sizeof(int) * 1024);
	TESTY_ASSERT(ptr != NULL);

	// Alloc with new block, 100 MB
	ptr = arena_alloc(&arena, 1024 * 1024 * 100);
	TESTY_ASSERT(ptr != NULL);

	arena_free(&arena);
	TESTY_ASSERT(arena.ptr == NULL);
	TESTY_ASSERT(arena.end == NULL);
	TESTY_ASSERT(arena.blocks == NULL);
}