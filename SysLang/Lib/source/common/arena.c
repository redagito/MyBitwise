#include "syl/common/arena.h"

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "syl/common/memory.h"
#include "syl/common/buffer.h"
#include "syl/common/log.h"

#define ARENA_ALIGNMENT 8
#define ARENA_BLOCK_SIZE (1024 * 1024)

// Align value
#define ALIGN_DOWN(n, a) ((n) & ~((a) - 1))
#define ALIGN_UP(n, a) ALIGN_DOWN((n) + (a) - 1, (a))

#define ALIGN_DOWN_PTR(p, a) ((void *)ALIGN_DOWN((uintptr_t)(p), (a)))
#define ALIGN_UP_PTR(p, a) ((void *)ALIGN_UP((uintptr_t)(p), (a)))

// Disables new block allocations
// Default false (0)
static bool arena_grow_disabled;

/**
* Internal API
* Grow arena by a minimum size
*/
static void arena_grow(arena_t* arena, size_t min_size)
{
	if (arena_grow_disabled)
	{
		log_fatal("Arena block allocation while grow is disabled");
	}

	size_t size = ALIGN_UP(max(min_size, ARENA_BLOCK_SIZE), ARENA_ALIGNMENT);
	arena->ptr = xmalloc(size);
	// Validate alignment
	assert(arena->ptr == ALIGN_DOWN_PTR(arena->ptr, ARENA_ALIGNMENT));

	// New end
	arena->end = arena->ptr + size;
	// Update blocks
	buffer_push(arena->blocks, arena->ptr);
}

static void* arena_alloc_internal(arena_t* arena, size_t size)
{
	assert(size <= (size_t)(arena->end - arena->ptr));

	// Pointer to start of free space on memory block
	void* ptr = arena->ptr;

	// Update ptr to free space by at least requested size
	arena->ptr = ALIGN_UP_PTR(arena->ptr + size, ARENA_ALIGNMENT);
	assert(arena->ptr <= arena->end);

	// Validate alignment
	assert(arena->ptr == ALIGN_DOWN_PTR(arena->ptr, ARENA_ALIGNMENT));
	assert(ptr == ALIGN_DOWN_PTR(ptr, ARENA_ALIGNMENT));

	return ptr;
}

arena_t arena_create()
{
	arena_t arena;
	arena.blocks = NULL;
	arena.end = NULL;
	arena.ptr = NULL;
	return arena;
}

void arena_disable_grow()
{
	arena_grow_disabled = true;
}

void arena_enable_grow()
{
	arena_grow_disabled = false;
}

void* arena_alloc(arena_t* arena, size_t size)
{
	if (size > (size_t)(arena->end - arena->ptr))
	{
		// Not enough space available
		arena_grow(arena, size);
	}
	return arena_alloc_internal(arena, size);
}

void* arena_alloc_no_grow(arena_t* arena, size_t size)
{
	if (size > (size_t)(arena->end - arena->ptr))
	{
		// Not enough space available
		// This function will not allocate new blocks
		log_fatal("Insufficient space on current memory block");
	}
	return arena_alloc_internal(arena, size);
}

void arena_free(arena_t* arena)
{
	// Delete memory blocks
	for (char** iter = arena->blocks; iter != buffer_end(arena->blocks); ++iter)
	{
		free(*iter);
	}
	// Delete blocks buffer
	buffer_free(arena->blocks);

	arena->ptr = NULL;
	arena->end = NULL;
	arena->blocks = NULL;
}