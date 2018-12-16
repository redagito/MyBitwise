#pragma once

#include <stdint.h>

/**
* Memory management
*/

void* xcalloc(size_t num_elements, size_t element_size);
void* xrealloc(void* ptr, size_t num_bytes);
void* xmalloc(size_t num_bytes);

/**
* Creates new memory with copy of provided data
*/
void* memdup(void* src, size_t size);