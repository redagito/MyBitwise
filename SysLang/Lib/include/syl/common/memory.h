#pragma once

#include <stdarg.h>
#include <stdint.h>

/**
 * Memory management
 */

/**
 * Allocates and sets memory to zero
 */
void *xcalloc(size_t num_elements, size_t element_size);
void *xrealloc(void *ptr, size_t num_bytes);
void *xmalloc(size_t num_bytes);

/**
 * Creates new memory with copy of provided data
 */
void *memdup(void *src, size_t size);

/**
 * Creates formatted string in new memory and retuns pointer
 */
char *strf(const char *fmt, ...);