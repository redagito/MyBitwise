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

char* strf(const char* fmt, ...)
{
	// Required size for the formatted string
	va_list args;
	va_start(args, fmt);
	size_t size = 1 + vsnprintf(NULL, 0, fmt, args);
	va_end(args);

	// Allocate and write string
	char* str = xmalloc(size);
	va_start(args, fmt);
	vsnprintf(str, size, fmt, args);
	va_end(args);

	return str;
}