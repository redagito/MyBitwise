#include "syl/common/memory.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void xnotnull(const void* ptr, const char* message)
{
	if (ptr == NULL)
	{
		perror(message);
		exit(1);
	}
}

void* xcalloc(size_t num_elements, size_t element_size)
{
	void* ptr = calloc(num_elements, element_size);
	xnotnull(ptr, "xcalloc failed");
	return ptr;
}

void* xrealloc(void* ptr, size_t num_bytes)
{
	ptr = realloc(ptr, num_bytes);
	xnotnull(ptr, "xrealloc failed");
	return ptr;
}

void* xmalloc(size_t num_bytes)
{
	void* ptr = malloc(num_bytes);
	xnotnull(ptr, "xmalloc failed");
	return ptr;
}

void* memdup(void* src, size_t size)
{
	void* dest = xmalloc(size);
	memcpy(dest, src, size);
	return dest;
}