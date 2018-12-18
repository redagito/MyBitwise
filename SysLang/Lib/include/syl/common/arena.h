#pragma once

#include <stdint.h>

/**
* Arena allocator
* An arena is a collection of memory blocks
* which get allocated individually but not freed/reclaimed
* except when freeing the whole arena.
*/

typedef struct arena_t
{
	char* ptr; // Start of free space in last memory block
	char* end; // End of last memory block
	char **blocks; // Array of allocated memory chunks
} arena_t;

/**
* Creates arena struct
* Sets default values.
*/
arena_t arena_create();

/**
* Disables/enables new block allocation on all arenas
* Affects subsequent calls to arena_alloc.
* Any call that causes a block alloc will result in a fatal error.
*/
void arena_disable_grow();
void arena_enable_grow();

/**
* Allocate new memory area on the arena
* This will either use an existing memory block or create a new one.
*/
void* arena_alloc(arena_t* arena, size_t size);

/**
* Allocates new memory area on the arena
* This will try to allocate memory on the current block or yield a fatal error.
* Use this to prevent new block allocations.
*/
void* arena_alloc_no_grow(arena_t* arena, size_t size);

/**
* Free the whole arena
*/
void arena_free(arena_t* arena);